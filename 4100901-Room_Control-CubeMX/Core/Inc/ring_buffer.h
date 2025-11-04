// Biblioteca de Buffer Circular (ring buffer)
// Proporciona una cola FIFO eficiente para comunicación entre interrupciones y lazo principal.
// Estrategia cuando el búfer está lleno: se sobrescribe el dato más antiguo (tail avanza).

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>

// Estructura del buffer circular
typedef struct {
    uint8_t *buffer;   // Memoria donde se almacenan los datos
    uint16_t head;     // Índice de escritura (posición del siguiente dato a escribir)
    uint16_t tail;     // Índice de lectura (posición del siguiente dato a leer)
    uint16_t capacity; // Tamaño total del buffer en bytes
    uint16_t count;    // Número de elementos actualmente almacenados
} ring_buffer_t;

// Inicializa el buffer circular con un arreglo externo y su capacidad
void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t capacity);
// Escribe un byte en el buffer; si está lleno, sobrescribe el más antiguo
bool ring_buffer_write(ring_buffer_t *rb, uint8_t data);
// Lee un byte del buffer; devuelve false si está vacío
bool ring_buffer_read(ring_buffer_t *rb, uint8_t *data);
// Devuelve la cantidad de elementos almacenados
uint16_t ring_buffer_count(ring_buffer_t *rb);
// Devuelve true si no hay datos para leer
bool ring_buffer_is_empty(ring_buffer_t *rb);
// Devuelve true si el buffer alcanzó su capacidad
bool ring_buffer_is_full(ring_buffer_t *rb);
// Limpia el contenido del buffer (lo deja vacío)
void ring_buffer_flush(ring_buffer_t *rb);

#endif // RING_BUFFER_H
