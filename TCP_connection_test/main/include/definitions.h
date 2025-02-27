/*
 * tasks_common.h
 *
 *  Created on: 24 de fev de 2023
 *      Author: gusta
 */

#ifndef MAIN_TASKS_COMMON_H_
#define MAIN_TASKS_COMMON_H_

#define PREAMBLE 0x5A
#define BOARD 7
#define VERSION 108 // v.3.0.2.27 XB3.0

#define DEBUG_MODE
// #define CPU_ONLY
// #define DEBUGSENSORS
// #define DEBUG_CMD
// #define MQTT_STATUS_DEBUG

#define INVALID_VALUE -999

// Gsm uart application task
#define UART_GSM_TASK_STACK_SIZE 16 * 1024
#define UART_GSM_TASK_PRIORITY configMAX_PRIORITIES - 1
#define UART_GSM_TASK_CORE_ID 0

// Coordinates application task
#define COORD_TASK_STACK_SIZE 8 * 1024
#define COORD_TASK_PRIORITY configMAX_PRIORITIES - 5
#define COORD_TASK_CORE_ID 0

// Gsm status task
#define GSM_STAT_TASK_STACK_SIZE 4 * 1024
#define GSM_STAT_TASK_PRIORITY configMAX_PRIORITIES - 10
#define GSM_STAT_TASK_CORE_ID 0

// MQTT connected task
#define MQTT_CON_TASK_STACK_SIZE 4 * 1024
#define MQTT_CON_TASK_PRIORITY configMAX_PRIORITIES - 10
#define MQTT_CON_TASK_CORE_ID 0

// Main application task
#define MAIN_TASK_STACK_SIZE 10 * 1024
#define MAIN_TASK_PRIORITY configMAX_PRIORITIES - 2
#define MAIN_TASK_CORE_ID 0

// Button task
#define BUTTON_TASK_STACK_SIZE 2 * 1024
#define BUTTON_TASK_PRIORITY 1
#define BUTTON_TASK_CORE_ID 0

// LED task
#define LED_TASK_STACK_SIZE 2 * 1024
#define LED_TASK_PRIORITY 1
#define LED_TASK_CORE_ID 0

// Sensor task
#define SENSOR_TASK_STACK_SIZE 4 * 1024
#define SENSOR_TASK_PRIORITY configMAX_PRIORITIES - 3
#define SENSOR_TASK_CORE_ID 1

// UART task
#define UART_TASK_STACK_SIZE 4 * 1024
#define UART_TASK_PRIORITY 20
#define UART_TASK_CORE_ID 0

// HTTP Server task
#define HTTP_SERVER_TASK_STACK_SIZE 8 * 1024
#define HTTP_SERVER_TASK_PRIORITY configMAX_PRIORITIES - 10
#define HTTP_SERVER_TASK_CORE_ID 0

// HTTP Server Monitor task
#define HTTP_SERVER_MONITOR_STACK_SIZE 4 * 1024
#define HTTP_SERVER_MONITOR_PRIORITY 3
#define HTTP_SERVER_MONITOR_CORE_ID 0
#endif /* MAIN_TASKS_COMMON_H_ */
