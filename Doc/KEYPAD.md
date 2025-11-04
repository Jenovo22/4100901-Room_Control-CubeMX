# Taller: Driver de Teclado Hexadecimal 4x4 (EXTI)

## Introducción

En esta guía implementarás un driver para un teclado matricial 4x4. En lugar de hacer sondeo continuo (polling), se usan interrupciones externas (EXTI): el micro se activa solo cuando hay una pulsación.

- Eficiente: el MCU puede hacer otras tareas o dormir.
- Reactivo: respuesta inmediata a la pulsación.
- Didáctico: práctica directa de interrupciones de hardware.

## 1. Configuración en STM32CubeMX

### 1.1 Filas (salidas)

Configura como `GPIO_Output`:

- `PA10` ? `KEYPAD_R1`
- `PB3`  ? `KEYPAD_R2`
- `PB5`  ? `KEYPAD_R3`
- `PB4`  ? `KEYPAD_R4`

### 1.2 Columnas (entradas con EXTI)

Configura como entrada con Pull-Up e interrupción por flanco de bajada:

- `PB10` ? `KEYPAD_C1`
- `PA8`  ? `KEYPAD_C2`
- `PA9`  ? `KEYPAD_C3`
- `PC7`  ? `KEYPAD_C4`

En `System Core > GPIO` para cada columna:

- `GPIO mode`: External Interrupt Mode with Falling edge trigger
- `Pull-up/Pull-down`: Pull-up

### 1.3 NVIC

Habilita:

- `EXTI line[9:5] interrupts` (PA8, PA9, PC7)
- `EXTI line[15:10] interrupts` (PB10)

## 2. Librería del Teclado

Archivo de cabecera (resumen):

```c
typedef struct {
    GPIO_TypeDef* row_ports[4];
    uint16_t row_pins[4];
    GPIO_TypeDef* col_ports[4];
    uint16_t col_pins[4];
} keypad_handle_t;

void keypad_init(keypad_handle_t* keypad);
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin);
```

Mapa de teclas (fila×columna): `1 2 3 A / 4 5 6 B / 7 8 9 C / * 0 # D`.

## 3. Lógica de Escaneo

1) Debounce: `HAL_Delay(5);`
2) Identificar columna: buscar `col_pin` en `keypad->col_pins` y obtener `col_index`.
3) Identificar fila:
   - Poner todas las filas en ALTO.
   - Bajar una fila a la vez y leer la columna.
   - Si lee BAJO, tecla detectada: `keypad_map[fila][col_index]`.
   - Esperar liberación (columna vuelva a ALTO) y salir.
4) Restaurar: llamar a `keypad_init()` para dejar filas en BAJO.
5) Retornar el carácter encontrado (o `\0`).

## 4. Integración en main.c

Includes:

```c
#include "keypad_driver.h"
#include "ring_buffer.h"
#include <stdio.h>
```

Estado global:

```c
keypad_handle_t keypad = { /* asigna puertos/pines de filas/columnas */ };
#define KEYPAD_BUFFER_LEN 16
uint8_t keypad_buffer[KEYPAD_BUFFER_LEN];
ring_buffer_t keypad_rb;
```

Callback EXTI y lazo principal (resumen):

```c
volatile uint16_t last_exti_pin = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    // Enmascarar EXTI y registrar pin para procesar fuera de la ISR
    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
    last_exti_pin = GPIO_Pin;
}

// En el while(1)
if (last_exti_pin != 0) {
    char key = keypad_scan(&keypad, last_exti_pin);
    last_exti_pin = 0;
    if (key != '\\0') ring_buffer_write(&keypad_rb, (uint8_t)key);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
```

## 5. Ejercicio Final

1) Completa `keypad_init()` y `keypad_scan()`.
2) Integra los archivos `.h` y `.c`.
3) Modifica `main.c` e imprime teclas por UART.
4) Compila, carga y prueba en Nucleo.
5) Control de acceso: espera 4 teclas, compara con clave y enciende/parpadea LED según resultado.

## Integración al Proyecto

El teclado hexadecimal es el principal medio de entrada del sistema “Control de Sala”. Se integra con el `ring_buffer` para manejo asíncrono y con UART para retroalimentación.

Siguiente paso: `Doc/SSD1306.md` (display OLED).