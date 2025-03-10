#ifndef PINS_H
#define PINS_H

#include "driver/gpio.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "simcom_definitions.h"
#include "driver/i2c.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define PIN_SIMCOM_RX GPIO_NUM_19
#define PIN_SIMCOM_TX GPIO_NUM_23

#define PIN_SDA GPIO_NUM_21
#define PIN_SCL GPIO_NUM_22

#define PIN_SPARE0 GPIO_NUM_39
#define PIN_CMD_BTN GPIO_NUM_35
#define PIN_LED GPIO_NUM_0

#define PIN_RST_MOD GPIO_NUM_34

#define PIN_SIMCOM_STATUS GPIO_NUM_5

// Default sensors adress
#define PCA9535_ADRRESS 0x22

// PCA9535 INP OUT CONFIG
#define PCA_INPUT 0x01
#define PCA_OUTPUT 0x00
#define SENSORS_CONFIG 0b00000000
#define IOEXP_CONFIG 0b10000010

// PCA9535 COMMANDS
#define PCA_INPUT0 0x00
#define PCA_INPUT1 0x01
#define PCA_OUTPU0 0x02
#define PCA_OUTPU1 0x03
#define PCA_POLARITY0 0x04
#define PCA_POLARITY1 0x05
#define PCA_CONFIG0 0x06
#define PCA_CONFIG1 0x07

void pins_init(void);

void set_pin(gpio_num_t pin, gpio_mode_t mode, gpio_int_type_t interrupt, gpio_isr_t isr, void *args);

bool simcom_get_pwr_mode(void);
bool pwrkey_power_on();
bool pwrkey_power_off();
bool gpio_pwrkey_write(bool value);
esp_err_t i2c_init(void);
esp_err_t pca9535_init();
esp_err_t pca9535_write(uint8_t command, uint8_t data);
esp_err_t pca9535_read(uint8_t command, uint8_t *data);

#endif // PINS_H