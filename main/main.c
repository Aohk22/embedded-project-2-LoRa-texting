#include "freertos/FreeRTOS.h"

#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include "my_lora.h"
#include "my_spi.h"
#include "portmacro.h"
#include <stdint.h>
#include <string.h>

static const char *TAG = "DEBUG";

void app_main(void) {
  char message[] = "Hello World!";
  uint8_t len = strlen(message);
  uint8_t i;
  uint8_t txBase;
  lora_config_t lr_cf = {0};

  lr_cf.regCommon.mode.longRange = 0b1; // long range mode.
  lr_cf.regCommon.mode.accessSharedReg = 0b0;
  lr_cf.regCommon.mode._ = 0b00;
  lr_cf.regCommon.mode.lowFrequency = 0b1;
  lr_cf.regCommon.mode.mode = 0b001;
  lr_cf.regCommon.frequency.msb = 0xe4;
  lr_cf.regCommon.frequency.mid = 0xc0;
  lr_cf.regCommon.frequency.lsb = 0x00;
  lr_cf.regPage.fifoRxBase = 0x00;
  lr_cf.regPage.fifoTxBase = 0x00;

  lora_init(&lr_cf);

  // debug section.
  ESP_LOGI(TAG, "lna: %x", lora_reg_read(REG_LNA));
  ESP_LOGI(TAG, "paConfig: %x", lora_reg_read(REG_PA_CONFIG));
  ESP_LOGI(TAG, "modemConfig3: %x", lora_reg_read(REG_MODEM_CONF_3));
  // end debug section.

  i = 0;
  txBase = lora_get_tx_base();
  while (1) {
    lora_set_fifo_ptr(txBase); // for spi.
    lora_set_tx_data(txBase, (uint8_t *)message, len);
    lora_set_payload_len(len);
    lora_set_fifo_ptr(txBase);
    ESP_LOGI(TAG, "setting TX mode.");
    lora_set_mode(MODE_TX);
    ESP_LOGI(TAG, "waiting for tx done flag...");
    while (!(lora_reg_read(REG_IRQ_FLAGS) & 1 << REG_IRQ_TX_DONE)) {
      ESP_LOGI(TAG, "polling flag.");
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "flag triggered, writing 1 to flag.");
    lora_reg_write(REG_IRQ_FLAGS, 1 << REG_IRQ_TX_DONE);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
