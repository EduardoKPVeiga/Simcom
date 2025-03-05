#ifndef PINS_H
#define PINS_H

#include "driver/gpio.h"
#include "esp_mac.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ESP_INTR_FLAG_DEFAULT 0

#define PIN_SS2 GPIO_NUM_12
#define PIN_SS3 GPIO_NUM_33
#define PIN_SS4 GPIO_NUM_32

#define PIN_SINCOM_RX GPIO_NUM_19
#define PIN_SINCOM_TX GPIO_NUM_23

#define PIN_SDA GPIO_NUM_21
#define PIN_SCL GPIO_NUM_22

#define PIN_SPARE0 GPIO_NUM_39
#define PIN_CMD_BTN GPIO_NUM_35
#define PIN_LED GPIO_NUM_0

#define PIN_RST_MOD GPIO_NUM_34

void pins_init(void);

void set_pin(gpio_num_t pin, gpio_mode_t mode, gpio_int_type_t interrupt, gpio_isr_t isr, void *args);

#endif // PINS_H