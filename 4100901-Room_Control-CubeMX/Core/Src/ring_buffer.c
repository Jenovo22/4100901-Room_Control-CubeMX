// Archivo: ring_buffer.c
// Implementación de un buffer circular simple para comunicación asíncrona.

#include "ring_buffer.h"

// Inicializa índices y contador, y asocia la memoria externa
void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t capacity) 
{
    rb->buffer = buffer;
    rb->capacity = capacity;
    rb->head = 0;   // próxima posición de escritura
    rb->tail = 0;   // próxima posición de lectura
    rb->count = 0;  // elementos almacenados
}

// Escribe un byte. Si está lleno, se sobrescribe el más antiguo (tail avanza)
bool ring_buffer_write(ring_buffer_t *rb, uint8_t data)
{
    // Escribe el dato en la posición de escritura actual
    rb->buffer[rb->head] = data;

    // Avanza la cabeza de forma circular
    rb->head = (rb->head + 1) % rb->capacity;

    if (rb->count < rb->capacity) {
        // Aumenta el conteo si aún no estaba lleno
        rb->count++;
    } else {
        // Si estaba lleno, al escribir se pisa el dato más antiguo.
        // Para mantener consistencia, avanzamos la cola (nuevo más antiguo)
        rb->tail = (rb->tail + 1) % rb->capacity;
    }
    
    return true;
}

// Lee un byte. Devuelve false si no hay datos
bool ring_buffer_read(ring_buffer_t *rb, uint8_t *data)
{
    if (rb->count == 0) {
        // No hay datos disponibles
        return false;
    }

    // Lee el dato más antiguo y avanza la cola
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count--;

    return true;
}

// Cantidad de elementos almacenados
uint16_t ring_buffer_count(ring_buffer_t *rb)
{
    return rb->count;
}

// Retorna true si no hay elementos
bool ring_buffer_is_empty(ring_buffer_t *rb)
{
    return rb->count == 0;
}

// Retorna true si count == capacity
bool ring_buffer_is_full(ring_buffer_t *rb)
{
    return rb->count == rb->capacity;
}

// Limpia el buffer (no borra la memoria, solo reinicia índices y contador)
void ring_buffer_flush(ring_buffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
    rb->count = 0;
}
