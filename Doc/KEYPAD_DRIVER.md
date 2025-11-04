# Driver de Teclado Matricial 4x4 (EXTI)

Este documento explica cómo funciona, se configura y se integra el driver del teclado 4x4 basado en interrupciones externas (EXTI) dentro del proyecto.

- Eficiencia: no se hace polling continuo; el MCU atiende solo cuando hay una pulsación.
- Reactividad: baja latencia, respuesta inmediata a la pulsación.
- Simplicidad: API pequeña y clara; fácil de portar a otros proyectos STM32.

## Hardware y Pines

- Filas (R1..R4): salidas GPIO.
- Columnas (C1..C4): entradas GPIO con `Pull-Up` y EXTI por flanco de bajada.
- Conexión típica: al presionar una tecla, se une una fila en BAJO con una columna que está en ALTO (por Pull-Up), provocando una transición a BAJO en la columna y disparando la interrupción.

Pines usados (según `Core/Inc/main.h`):

- Filas: `PA10 (KEYPAD_R1)`, `PB3 (KEYPAD_R2)`, `PB5 (KEYPAD_R3)`, `PB4 (KEYPAD_R4)`.
- Columnas: `PB10 (KEYPAD_C1)`, `PA8 (KEYPAD_C2)`, `PA9 (KEYPAD_C3)`, `PC7 (KEYPAD_C4)`.

## Configuración en STM32CubeMX

1) GPIO filas: `GPIO_Output` (Push-Pull), velocidad baja, sin Pull.
2) GPIO columnas: `GPIO_MODE_IT_FALLING` y `GPIO_PULLUP`.
3) NVIC: habilitar `EXTI line[9:5]` y `EXTI line[15:10]`.

El proyecto ya refleja esto en `Core/Src/main.c` dentro de `MX_GPIO_Init()`.

## Arquitectura del Driver

- Filas en BAJO cuando el sistema está en reposo (prepara la detección de flanco de bajada en columnas).
- Al generarse una EXTI por una columna, en la ISR solo se registra el pin y se enmascaran ambos grupos EXTI (9:5 y 15:10). El escaneo real ocurre en el lazo principal.
- Escaneo: se identifican fila y columna activas y se devuelve el carácter correspondiente.

Archivos relacionados:

- `Core/Inc/keypad_driver.h`: definición del handle y prototipos.
- `Core/Src/keypad_driver.c`: implementación de `keypad_init()` y `keypad_scan()`.
- `Core/Src/main.c`: integración (buffer, ISR, lazo principal, lógica de clave).

## API del Teclado

Header: `Core/Inc/keypad_driver.h`

- `typedef struct keypad_handle_t`:
  - `row_ports[4]`, `row_pins[4]`: puertos/pines de filas.
  - `col_ports[4]`, `col_pins[4]`: puertos/pines de columnas.
- `void keypad_init(keypad_handle_t* keypad)`:
  - Deja todas las filas en BAJO, listo para detectar flancos en columnas.
- `char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin)`: 
  - Escanea tras una EXTI. Devuelve el carácter presionado o `\0` si no se detecta.

Mapa de teclas (fila x columna):

```
1 2 3 A
4 5 6 B
7 8 9 C
* 0 # D
```

## Flujo de Interrupciones y Escaneo

1) Columna detecta flanco de bajada -> EXTI.
2) `HAL_GPIO_EXTI_Callback`:
   - Enmascara `EXTI9_5` y `EXTI15_10` para evitar reentradas.
   - Registra el pin en `last_exti_pin`.
3) En el lazo principal (`while(1)`):
   - Si `last_exti_pin != 0`, llama a `keypad_scan()`.
   - Si hay tecla válida, la escribe en el `ring_buffer`.
   - Reactiva `EXTI9_5` y `EXTI15_10` para próximas pulsaciones.

Rebote y temporización:

- Debounce de ~5 ms antes de escanear.
- Tras bajar cada fila, se espera ~1 ms antes de leer la columna (estabilización).
- Se bloquea hasta que la columna vuelva a ALTO (con pausas de 1 ms) para evitar repeticiones.

## Integración con Ring Buffer

Se utiliza un buffer circular para desacoplar la ISR del procesamiento en el lazo principal:

- `ring_buffer_init(&keypad_rb, keypad_buffer, KEYPAD_BUFFER_LEN);`
- En ISR -> se marca el pin; fuera de la ISR -> `keypad_scan()` -> `ring_buffer_write()`.
- El lazo principal lee con `ring_buffer_read()` y procesa (p. ej., verificar una clave).

Referencias:

- Teclado: `Core/Src/keypad_driver.c`
- Buffer: `Core/Src/ring_buffer.c`, `Core/Inc/ring_buffer.h`
- Integración: `Core/Src/main.c`

## Ejemplo de Uso (resumen)

En `Core/Src/main.c`:

```
// 1) Inicialización
ring_buffer_init(&keypad_rb, keypad_buffer, KEYPAD_BUFFER_LEN);
keypad_init(&keypad);

// 2) ISR
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    last_exti_pin = GPIO_Pin;
}

// 3) Lazo principal
if (last_exti_pin != 0) {
    char key = keypad_scan(&keypad, last_exti_pin);
    last_exti_pin = 0;
    if (key != '\0') ring_buffer_write(&keypad_rb, (uint8_t)key);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

uint8_t k;
if (ring_buffer_read(&keypad_rb, &k)) {
    // procesar tecla k
}
```

## Problemas Comunes y Solución

- No detecta pulsación o se detecta al soltar:
  - Verificar que columnas estén en `GPIO_MODE_IT_FALLING` con Pull-Up.
  - Confirmar que `keypad_init()` deja filas en BAJO.
- Doble lectura o rebotes:
  - Ajustar `HAL_Delay(5)` (debounce) en `keypad_scan()`.
  - Confirmar espera de liberación (columna vuelve a ALTO).
- Interrupciones repetitivas durante escaneo:
  - Asegurar enmascarado de `EXTI9_5` y `EXTI15_10` en el callback y reactivación posterior.

## Pruebas Rápidas

1) Pulsar cada tecla y verificar la impresión por UART.
2) Mantener pulsada una tecla: se debe leer una sola vez.
3) Probar presiones rápidas y combinaciones diagonales: no debe haber lecturas fantasma.
4) Verificar lógica de clave (4 dígitos) y respuesta del LED.

## Extensiones

- Repetición por tecla (auto-repeat) con temporizador.
- Detección de “tecla larga” vs “tecla corta”.
- Mapas de teclas personalizados.

