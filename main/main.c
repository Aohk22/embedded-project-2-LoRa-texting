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
  uint8_t opMode, rxBase, txBase;
  uint32_t freq;
  lora_config_t lr_cf;

  lr_cf.regCommon.mode.longRange = 0b1;
  lr_cf.regCommon.mode.accessSharedReg = 0b0;
  lr_cf.regCommon.mode._ = 0b00;
  lr_cf.regCommon.mode.lowFrequency = 0b1;
  lr_cf.regCommon.mode.mode = 0b001;
  lr_cf.regCommon.frequency.msb = 0xe4;
  lr_cf.regCommon.frequency.mid = 0xc0;
  lr_cf.regCommon.frequency.lsb = 0x00;

  lr_cf.regPage.fifoRxBase = 0x00;
  lr_cf.regPage.fifoTxBase = 0x80;

  // lr_cf.regRfBlocks.

  lora_init(&lr_cf);

  txBase = lora_get_tx_base();
  lora_set_fifo_ptr(txBase); // for writes auto increment.
  lora_set_tx_data(txBase, (uint8_t *)message, len);
  lora_set_payload_len(len);
  // lora_set_mode(MODE_TX);
  ESP_LOGI(TAG, "fifoPtr %x", lora_get_fifo_ptr());
  uint8_t dbugStr[13];
  // fifo e.g. queue so no need for incing addr.
  // now fifo mode makes sense.
  uint8_t addr = 0x80;
  for (int i = 0; i < 12; i++) {
    lora_reg_read(addr, &dbugStr[i]);
  }
  ESP_LOGI(TAG, "in fifo: %s", dbugStr);

  i = 0;
  while (1) {
    // uint8_t paRamp;
    // uint8_t irqFlags = 0;
    // uint8_t flagTxDone;
    // uint8_t fifoPtr;
    // ESP_LOGI(TAG, "loop number %d -------", i);
    // lora_reg_read(REG_OP_MODE, &opMode);
    // lora_reg_read(REG_PA_RAMP, &paRamp);
    // lora_reg_read(REG_IRQ_FLAGS, &flagTxDone);
    // lora_reg_read(REG_FIFO_ADDR, &fifoPtr);
    // lora_set_mode(MODE_TX);
    // flagTxDone = irqFlags & 0x08;
    // if (flagTxDone) { // poll flag
    //   lora_reg_write(REG_IRQ_FLAGS, 0x08);
    // }
    //
    // freq = lora_get_frequency();
    // rxBase = lora_get_rx_base();
    //
    // // ESP_LOGI(TAG, "paRamp %x", paRamp);
    // ESP_LOGI(TAG, "RegOpMode %x", opMode);
    // ESP_LOGI(TAG, "txDone %x", flagTxDone);
    // ESP_LOGI(TAG, "fifoPtr after %x", txBase);
    // i++;
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}
