#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "main_task.h"

extern "C"
{
    void app_main(void)
    {
        main_task_init();
    }
}
