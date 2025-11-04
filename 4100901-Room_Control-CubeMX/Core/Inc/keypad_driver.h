#ifndef KEYPAD_DRIVER_H
#define KEYPAD_DRIVER_H

#include "main.h"
#include <stdint.h>

// Dimensiones del teclado 4x4
#define KEYPAD_ROWS 4
#define KEYPAD_COLS 4

// Manejador del teclado: define los puertos y pines de filas/columnas
typedef struct {
    GPIO_TypeDef* row_ports[KEYPAD_ROWS]; // Puertos de las filas (R1..R4)
    uint16_t row_pins[KEYPAD_ROWS];       // Pines de las filas
    GPIO_TypeDef* col_ports[KEYPAD_COLS]; // Puertos de las columnas (C1..C4)
    uint16_t col_pins[KEYPAD_COLS];       // Pines de las columnas
} keypad_handle_t;

// Deja filas en BAJO para que una pulsación genere flanco de bajada en columna
void keypad_init(keypad_handle_t* keypad);
// Escanea la tecla asociada al pin de columna que interrumpió; retorna '\0' si nada
char keypad_scan(keypad_handle_t* keypad, uint16_t col_pin);

#endif // KEYPAD_DRIVER_H
