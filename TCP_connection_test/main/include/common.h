#ifndef COMMON_H_
#define COMMON_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

#include "esp_log.h"
#include "esp_event.h"
#include "esp_event_base.h"
#include "esp_check.h"
#include "esp_system.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_wifi.h"
#include "esp_adc_cal.h"
#include "esp_err.h"
#include "esp_timer.h"
#include "esp_sleep.h"
#include "esp_eth_driver.h"
#include "esp_http_server.h"
#include "esp_ota_ops.h"
#include "esp_rom_gpio.h"
#include "esp_intr_alloc.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "esp_spiffs.h"
#include <esp_task_wdt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctype.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <dirent.h>

#include <sys/time.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "cJSON.h"

#include "driver/spi_common.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "driver/adc.h"
#include "driver/dac.h"
#include "driver/rtc_io.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"
#include "driver/timer.h"

#include "sdkconfig.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/ip4_addr.h"
#include "lwip/apps/sntp.h"
#include <lwip/netdb.h>

#include "mqtt_client.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "rom/ets_sys.h"

#include "soc/timer_group_struct.h"

#include "definitions.h"

#endif /* COMMON_H_ */