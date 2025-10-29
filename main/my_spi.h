#include <stdint.h>

// change these if want.
#define MISO 12
#define MOSI 13
#define CLK 14
#define CS 15

void spi_init();
void spi_send_byte(uint8_t data);
void spi_recv_byte(uint8_t *recv);
