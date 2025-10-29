#include "my_spi.h"
#include "driver/gpio.h"
#include <stdint.h>

void spi_init() {
  gpio_config_t gp_con;
  gp_con.intr_type = GPIO_INTR_DISABLE, gp_con.mode = GPIO_MODE_OUTPUT,
  gp_con.pin_bit_mask = (1ULL << MOSI) | (1ULL << CLK) | (1ULL << CS),
  gp_con.pull_down_en = 0, gp_con.pull_up_en = 0;
  gpio_config(&gp_con);

  gp_con.mode = GPIO_MODE_INPUT;
  gp_con.pin_bit_mask = (1ULL << MISO);
  gpio_config(&gp_con);

  // clock low, chip select high.
  gpio_set_level(CS, 1);
  gpio_set_level(CLK, 0);
}

void spi_send_byte(uint8_t data) {
  for (int i = 7; i >= 0; i--) {
    gpio_set_level(MOSI, (data >> i) & 1);
    gpio_set_level(CLK, 1);
    gpio_set_level(CLK, 0);
  }
}

void spi_recv_byte(uint8_t *recv) {
  for (int i = 7; i >= 0; i--) {
    gpio_set_level(CLK, 1);
    if (gpio_get_level(MISO))
      *recv |= (1 << i);
    gpio_set_level(CLK, 0);
  }
}
