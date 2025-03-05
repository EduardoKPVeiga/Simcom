#include "pins.h"

uint8_t ioexp = 0;
SemaphoreHandle_t sensors_semaphore;

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

    sensors_semaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(sensors_semaphore);
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

bool simcom_get_pwr_mode(void)
{
    return gpio_get_level(PIN_SIMCOM_STATUS) == 0 ? true : false;
}

bool pwrkey_power_on()
{
    if (simcom_get_pwr_mode() == false)
    {
        gpio_pwrkey_write(false);
        vTaskDelay(PWRKEY_T_ON * 5);
        gpio_pwrkey_write(true);
        vTaskDelay(PWRKEY_T_ON);
        gpio_pwrkey_write(false);
        vTaskDelay(PWRKEY_T_ON_UART - PWRKEY_T_ON);
    }

    uint16_t c = 0;
    while (simcom_get_pwr_mode() == false)
    {
        c++;
        vTaskDelay(SIMCOM_PWR_ON_DELAY / portTICK_PERIOD_MS);
        if (c >= SIMCOM_PWR_ON_ATTEMPTS)
            return false;
    }
    return true;
}

bool pwrkey_power_off()
{
    if (simcom_get_pwr_mode() == true)
    {
        vTaskDelay(PWRKEY_T_OFF);
        gpio_pwrkey_write(true);
        vTaskDelay(PWRKEY_T_OFF);
        gpio_pwrkey_write(false);
        vTaskDelay(PWRKEY_T_OFF_STATUS);
    }

    uint16_t c = 0;
    while (simcom_get_pwr_mode() == true)
    {
        c++;
        vTaskDelay(GSM_PWR_OFF_DELAY / portTICK_PERIOD_MS);
        if (c >= GSM_PWR_OFF_ATTEMPTS)
            return false;
    }
    return true;
}

bool gpio_pwrkey_write(bool value)
{
    // RST_ETH
    if (value)
        ioexp |= 0b00000001;
    else
        ioexp &= ~(0b00000001);
    return pca9535_write(PCA_OUTPU1, ioexp) == ESP_OK ? true : false;
}

esp_err_t pca9535_init()
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    // waits for semaphore
    if (xSemaphoreTake(sensors_semaphore, 1000 / portTICK_PERIOD_MS) == pdTRUE)
    {
        // read data from sensor
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (PCA9535_ADRRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        error = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS); // HERE
        i2c_cmd_link_delete(cmd);

        // time for execution to complete
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // gives back semaphore
        xSemaphoreGive(sensors_semaphore);
    }

    if (error != ESP_OK)
    {
        return error;
    }

    error = pca9535_write(PCA_CONFIG0, SENSORS_CONFIG);
    error = pca9535_write(PCA_CONFIG1, IOEXP_CONFIG);

    uint8_t turnOff_gpios = 0x00;
    uint8_t turnOn_gpios = 0xFF;
    error = pca9535_write(PCA_OUTPU0, turnOn_gpios);
    error = pca9535_write(PCA_OUTPU1, turnOff_gpios);

    return error;
}

esp_err_t pca9535_write(uint8_t command, uint8_t data)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    // waits for semaphore
    if (xSemaphoreTake(sensors_semaphore, 1000 / portTICK_PERIOD_MS) == pdTRUE)
    {
        // read data from sensor
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (PCA9535_ADRRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_NACK);
        i2c_master_write_byte(cmd, command, I2C_MASTER_NACK);
        i2c_master_write_byte(cmd, data, I2C_MASTER_NACK);
        // i2c_master_write_byte(cmd, ioexp, I2C_MASTER_NACK);
        i2c_master_stop(cmd);
        error = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        // time for execution to complete
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // gives back semaphore
        xSemaphoreGive(sensors_semaphore);
    }
    return error;
}

esp_err_t pca9535_read(uint8_t command, uint8_t *data)
{
    esp_err_t error = ESP_ERR_TIMEOUT;
    if (xSemaphoreTake(sensors_semaphore, 1000 / portTICK_PERIOD_MS) == pdTRUE)
    {
        // send the command
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (PCA9535_ADRRESS << 1) | I2C_MASTER_WRITE, I2C_MASTER_NACK);
        i2c_master_write_byte(cmd, command, I2C_MASTER_NACK);
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (PCA9535_ADRRESS << 1) | I2C_MASTER_READ, I2C_MASTER_NACK);
        i2c_master_read(cmd, data, 1, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        error = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
        i2c_cmd_link_delete(cmd);

        // time for execution to complete
        vTaskDelay(10 / portTICK_PERIOD_MS);

        // gives back semaphore
        xSemaphoreGive(sensors_semaphore);
    }
    return error;
}
