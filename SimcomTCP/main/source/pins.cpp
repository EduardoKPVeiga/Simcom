#include "pins.h"

void pins_init(void)
{
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    set_pin(PIN_SPARE0, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_CMD_BTN, GPIO_MODE_INPUT, GPIO_INTR_ANYEDGE, NULL, NULL);
    set_pin(PIN_LED, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SCL, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SDA, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SS2, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SS3, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_SS4, GPIO_MODE_OUTPUT, GPIO_INTR_DISABLE, NULL, NULL);
    set_pin(PIN_RST_MOD, GPIO_MODE_INPUT, GPIO_INTR_DISABLE, NULL, NULL);

    vTaskDelay(1 / portTICK_PERIOD_MS);
    gpio_set_level(PIN_LED, true);
}

void set_pin(gpio_num_t pin, gpio_mode_t mode, gpio_int_type_t interrupt, gpio_isr_t isr, void *args)
{
    esp_rom_gpio_pad_select_gpio(pin);
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_direction(pin, mode));

    if (interrupt != GPIO_INTR_DISABLE)
    {
        ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_intr_type(pin, interrupt));
        ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_isr_handler_add(pin, isr, args));
    }
}