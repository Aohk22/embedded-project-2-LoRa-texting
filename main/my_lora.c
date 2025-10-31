#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "freertos/semphr.h"
#include "my_lora.h"
#include "my_spi.h"
#include "portmacro.h"

#include <stdint.h>
#include <sys/types.h>

static const char *TAG_LORA = "LORA";

void lora_init(lora_config_t *lr_cf) {
  spi_init();
  // put in sleep mode first to change the LongRangeMode bit.
  lora_reg_write(REG_OP_MODE, lr_cf->regCommon.mode.val & 0b11111000);
  lora_reg_write(REG_OP_MODE, lr_cf->regCommon.mode.val);

  lora_reg_write(REG_FR_MSB, lr_cf->regCommon.frequency.msb);
  lora_reg_write(REG_FR_MID, lr_cf->regCommon.frequency.mid);
  lora_reg_write(REG_FR_LSB, lr_cf->regCommon.frequency.lsb);

  lora_reg_write(REG_FIFO_RX_BASE, lr_cf->regPage.fifoRxBase);
  lora_reg_write(REG_FIFO_TX_BASE, lr_cf->regPage.fifoTxBase);

  lora_reg_write(REG_LNA, lr_cf->regRfBlocks.lna.val);
  lora_reg_write(REG_PA_CONFIG, lr_cf->regRfBlocks.paConfig.val);
  lora_reg_write(REG_MODEM_CONF_3, lr_cf->regPage.modemConfig3.val);
}

void lora_recv(uint8_t **recvArr) {}

void lora_reg_write(uint8_t addr, uint8_t data) {
  gpio_set_level(CS, 0);
  spi_send_byte(1 << 7 | addr);
  spi_send_byte(data);
  gpio_set_level(CS, 1);
}

uint8_t lora_reg_read(uint8_t addr) {
  uint8_t recv = 0;
  gpio_set_level(CS, 0);
  spi_send_byte(0x7f & addr);
  spi_recv_byte(&recv);
  gpio_set_level(CS, 1);
  return recv;
}

void lora_set_tx_data(uint8_t txAddr, uint8_t *transArr, uint8_t l) {
  // mem addr should inc (auto), but fifo pointer for spi should stay the same.
  gpio_set_level(CS, 0);
  spi_send_byte(0x80 | txAddr);
  for (int i = 0; i < l; i++) {
    spi_send_byte(*(transArr + i));
  }
  gpio_set_level(CS, 1);
}

void lora_set_mode(lora_device_modes_t mode) {
  uint8_t curr_reg = lora_get_mode();
  curr_reg = (curr_reg & 0b11111000) | (mode & 0b111);
  lora_reg_write(REG_OP_MODE, curr_reg);
}

void lora_set_fifo_ptr(uint8_t addr) { lora_reg_write(REG_FIFO_ADDR, addr); }

void lora_set_payload_len(uint8_t len) { lora_reg_write(REG_PAYLOAD_LEN, len); }

uint32_t lora_get_frequency() {
  uint8_t msb, mid, lsb;
  msb = lora_reg_read(REG_FR_MSB);
  mid = lora_reg_read(REG_FR_MID);
  lsb = lora_reg_read(REG_FR_LSB);
  return (msb << 8 * 2) | (mid << 8) | lsb;
}

uint8_t lora_get_rx_base(void) { return lora_reg_read(REG_FIFO_RX_BASE); }

uint8_t lora_get_tx_base(void) { return lora_reg_read(REG_FIFO_TX_BASE); }

uint8_t lora_get_mode(void) { return lora_reg_read(REG_OP_MODE); }

uint8_t lora_get_fifo_ptr(void) { return lora_reg_read(REG_FIFO_ADDR); }
