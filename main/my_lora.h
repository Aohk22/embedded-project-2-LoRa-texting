#include <stdint.h>

#define REG_FIFO 0x00
#define REG_OP_MODE 0x01
#define REG_FR_MSB 0x06
#define REG_FR_MID 0x07
#define REG_FR_LSB 0x08

#define REG_FIFO_ADDR 0x0d
#define REG_FIFO_TX_BASE 0x0e
#define REG_FIFO_RX_BASE 0x0f
#define REG_FIFO_RX_ADDR 0x10

#define REG_PA_CONFIG 0X09
#define REG_PA_RAMP 0x0a
#define REG_OCP 0x0b
#define REG_LNA 0x0c

#define REG_IRQ_FLAGS 0x12

#define REG_PAYLOAD_LEN 0x22

typedef enum {
  MODE_SLEEP = 0,
  MODE_STDBY,
  MODE_FSTX,
  MODE_TX,
  MODE_FSRX,
  MODE_RXCONTINUOUS,
  MODE_RXSINGLE,
  MODE_CAD,
} lora_device_modes_t;

typedef union {
  uint8_t val;
  struct {
    uint8_t mode : 3;
    uint8_t lowFrequency : 1;
    uint8_t _ : 2; // reserved.
    uint8_t accessSharedReg : 1;
    uint8_t longRange : 1;
  };
} lora_mode_t;

typedef struct {
  uint8_t msb;
  uint8_t mid;
  uint8_t lsb;
} lora_freq_t;

typedef union {
  uint8_t value;
  struct {
    uint8_t cadDetected : 1;
    uint8_t fhssChangeChannel : 1;
    uint8_t cadDone : 1;
    uint8_t txDone : 1;
    uint8_t validHeader : 1;
    uint8_t payloadCrcError : 1;
    uint8_t rxDone : 1;
    uint8_t rxTimeout : 1;
  };
} lora_irqf_t;

typedef struct {
  lora_mode_t mode;
  lora_freq_t frequency;
} lora_common_t;

typedef struct {
  uint8_t fifoAddrPtr;
  uint8_t fifoTxBase;
  uint8_t fifoRxBase;
  uint8_t fifoRxCurr;
  lora_irqf_t irqFlagMask;
} lora_page_t;

typedef struct {
  union {
    uint8_t val;
    struct {
      uint8_t paSelect : 1;
      uint8_t maxPower : 3;
      uint8_t outputPower : 4;
    };
  } paConfig;
  union {
    uint8_t val;
    struct {
      uint8_t _unused : 3;
      uint8_t _reserved : 1;
      uint8_t paRamp : 4;
    };
  } paRamp;
  union {
    uint8_t val;
    struct {
      uint8_t _unused : 2;
      uint8_t ocpOn : 1;
      uint8_t ocpTrim : 5;
    };
  } ocp;

  union {
    uint8_t val;
    struct {
      uint8_t lnaGain : 3;
      uint8_t lnaBoostLf : 2;
      uint8_t _reserved : 1;
      uint8_t lnaBoostHf : 2;
    };
  } lna;
} lora_rf_blocks_t;

typedef struct {
  lora_common_t regCommon;
  lora_page_t regPage;
  lora_rf_blocks_t regRfBlocks;
} lora_config_t;

void lora_init(lora_config_t *lr_cf);

void lora_recv(uint8_t **recvArr);

void lora_reg_write(uint8_t addr, uint8_t data);
void lora_reg_read(uint8_t addr, uint8_t *recv);
void lora_set_mode(lora_device_modes_t mode);
void lora_set_tx_data(uint8_t txAddr, uint8_t *transArr, uint8_t l);
void lora_set_fifo_ptr(uint8_t addr);
void lora_set_payload_len(uint8_t len);
uint32_t lora_get_frequency(void);
uint8_t lora_get_rx_base(void);
uint8_t lora_get_tx_base(void);
uint8_t lora_get_mode(void);
uint8_t lora_get_pa_ramp(void);
uint8_t lora_get_fifo_ptr(void);
