#include "main_task.h"
#include "esp_log.h"

#define TAG "main"

void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);
    ESP_LOGI(TAG, "Application Init!");
    main_task_init();
}