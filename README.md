# Segunda Fase del Curso: Estructuras Computacionales

**Universidad Nacional de Colombia - Sede Manizales**

**Curso:** Estructuras Computacionales (4100901)

## Introducción

En esta segunda fase del curso Estructuras Computacionales, nos enfocamos en la implementación de los componentes fundamentales de un dispositivo computacional. El propósito principal es fortalecer habilidades en la creación y reutilización de drivers/librerías, así como en la integración de componentes de entrada/salida (I/O).

Los conocimientos se aplican progresivamente en el proyecto final “Control de Sala”, un sistema embebido que integra:

- Un teclado hexadecimal para entrada de comandos.
- Un display OLED (SSD1306) para visualización de estado.
- Un sensor de temperatura para monitoreo ambiental.
- Un sistema de ventilación controlado por PWM.
- Conexión a Internet mediante el módulo ESP01 para telemetría.

## Objetivos Específicos

- Comprender y dominar STM32CubeMX para la configuración inicial.
- Diseñar y organizar librerías modulares en C.
- Implementar GPIO, UART, Timers, buffers circulares y periféricos (ADC, PWM, I2C).
- Integrar componentes para construir sistemas robustos y reutilizables.
- Aplicar técnicas de optimización en memoria, tiempo de ejecución y consumo energético.

## Guías Progresivas del Proyecto “Control de Sala”

Este repositorio contiene guías secuenciales que construyen el proyecto paso a paso:

- `Doc/SETUP.md`: Configuración del entorno de desarrollo.
- `Doc/CUBEMX_CONFIG.md`: Configuración de STM32CubeMX y periféricos básicos (GPIOs, UART, NVIC).
- `Doc/LIB_PRINCIPLES.md`: Principios de librerías modulares y programación estructurada.
- `Doc/RING_BUFFER.md`: Implementación de buffer circular para UART.
- `Doc/KEYPAD.md`: Taller del teclado hexadecimal (sesión: Teclado).
- `Doc/KEYPAD_DRIVER.md`: Documentación técnica del driver de teclado 4x4 por EXTI.
- `Doc/SSD1306.md`: Integración del display OLED.
- `Doc/OPTIMIZATION.md`: Técnicas de optimización (memoria, tiempo, energía).
- `Doc/SENSOR.md`: Sensor de temperatura.
- `Doc/FAN.md`: Control de ventilador por PWM.
- `Doc/ESP01.md`: Conexión IoT con ESP01.

Cada guía incluye ejercicios prácticos, integración al proyecto “Control de Sala” y el siguiente paso sugerido.

---

¡Manos a la obra! Comienza por revisar la Configuración del Entorno: `Doc/SETUP.md`.

---

## Resumen de Documentación

- Teclado 4x4 (conceptos y taller): `Doc/KEYPAD.md`
- Teclado 4x4 (API, flujo EXTI, integración): `Doc/KEYPAD_DRIVER.md`
- Buffer circular (API y uso con UART/teclado): `Doc/RING_BUFFER.md`
- Display OLED SSD1306: `Doc/SSD1306.md`
- Sensor de temperatura: `Doc/SENSOR.md`
- Ventilador PWM: `Doc/FAN.md`
- Optimización: `Doc/OPTIMIZATION.md`
- Configuración CubeMX: `Doc/CUBEMX_CONFIG.md`
- Entorno y setup: `Doc/SETUP.md`

## Índice de Prácticas

1. Entorno y setup: `Doc/SETUP.md`
2. Configuración inicial y CubeMX: `Doc/CUBEMX_CONFIG.md`
3. Principios de librerías modulares: `Doc/LIB_PRINCIPLES.md`
4. Buffer circular (UART/Teclado): `Doc/RING_BUFFER.md`
5. Teclado hexadecimal (taller): `Doc/KEYPAD.md`
6. Driver de teclado (API/flujo EXTI): `Doc/KEYPAD_DRIVER.md`
7. Display OLED SSD1306: `Doc/SSD1306.md`
8. Sensor de temperatura: `Doc/SENSOR.md`
9. Ventilador PWM: `Doc/FAN.md`
10. Conectividad IoT (ESP01): `Doc/ESP01.md`
11. Optimización (memoria, tiempo, energía): `Doc/OPTIMIZATION.md`

## Estructura del Proyecto (general)

- `Core/Inc`: Headers de la aplicación y drivers (p. ej., `keypad_driver.h`, `ring_buffer.h`).
- `Core/Src`: Implementaciones (p. ej., `main.c`, `keypad_driver.c`, `ring_buffer.c`).
- `Drivers`: HAL/CMSIS STM32.
- `Doc`: Guías y referencias del proyecto.
- `cmake`: Presets y toolchains para construir con CMake.

## Compilación rápida (CMake)

1. Crear carpeta de build (por ejemplo `build/`).
2. Configurar con el preset adecuado (ver `CMakePresets.json`).
3. Compilar: `cmake --build build --config Release`.

Para detalles y capturas, ver `Doc/SETUP.md` y `Doc/CUBEMX_CONFIG.md`.