#ifndef CCB35EB4_6D9A_477B_8D84_B36D42DE88BB
#define CCB35EB4_6D9A_477B_8D84_B36D42DE88BB

#include <Arduino.h>

#define PS2BLE_LOG_LEVEL_NONE 0
#define PS2BLE_LOG_LEVEL_ERROR 1
#define PS2BLE_LOG_LEVEL_WARN 2
#define PS2BLE_LOG_LEVEL_INFO 3
#define PS2BLE_LOG_LEVEL_DEBUG 4
#define PS2BLE_LOG_LEVEL_VERBOSE 5

#ifndef PS2BLE_LOG_LEVEL
#define PS2BLE_LOG_LEVEL PS2BLE_LOG_LEVEL_NONE
#endif

#ifndef PS2BLE_LOG_SERIAL
#define PS2BLE_LOG_SERIAL (Serial)
#endif

#ifndef PS2BLE_LOG_TASK_PRIORITY
#define PS2BLE_LOG_TASK_PRIORITY 0
#endif

#ifndef PS2BLE_LOG_TASK_CORE
#define PS2BLE_LOG_TASK_CORE APP_CPU_NUM
#endif

#ifndef PS2BLE_LOG_QUEUE_SIZE
#define PS2BLE_LOG_QUEUE_SIZE 20
#endif

#define PS2BLE_LOG_START()                                                                                        \
  do {                                                                                                            \
    xQueueSerialOutput = xQueueCreate(PS2BLE_LOG_QUEUE_SIZE, sizeof(std::string*));                               \
    xTaskCreateUniversal(taskSerial, "Serial", 4096, NULL, PS2BLE_LOG_TASK_PRIORITY, NULL, PS2BLE_LOG_TASK_CORE); \
  } while (0)

// clang-format off
#define PS2BLE_LOGE(str) do {} while (0)
#define PS2BLE_LOGW(str) do {} while (0)
#define PS2BLE_LOGI(str) do {} while (0)
#define PS2BLE_LOGD(str) do {} while (0)
#define PS2BLE_LOGV(str) do {} while (0)
// clang-format on

#if PS2BLE_LOG_LEVEL >= PS2BLE_LOG_LEVEL_ERROR
#undef PS2BLE_LOGE
#define PS2BLE_LOGE(str) serialPrintln(str)
#endif
#if PS2BLE_LOG_LEVEL >= PS2BLE_LOG_LEVEL_WARN
#undef PS2BLE_LOGW
#define PS2BLE_LOGW(str) serialPrintln(str)
#endif
#if PS2BLE_LOG_LEVEL >= PS2BLE_LOG_LEVEL_INFO
#undef PS2BLE_LOGI
#define PS2BLE_LOGI(str) serialPrintln(str)
#endif
#if PS2BLE_LOG_LEVEL >= PS2BLE_LOG_LEVEL_DEBUG
#undef PS2BLE_LOGD
#define PS2BLE_LOGD(str) serialPrintln(str)
#endif
#if PS2BLE_LOG_LEVEL >= PS2BLE_LOG_LEVEL_VERBOSE
#undef PS2BLE_LOGV
#define PS2BLE_LOGV(str) serialPrintln(str)
#endif

void serialPrint(std::string str);
void serialPrint(const char* str);
void serialPrint(char c);
void serialPrintln(std::string str);
void serialPrintln(const char* str);
void serialPrintln(char c);
void serialPrintln(void);
void taskSerial(void* arg);

extern QueueHandle_t xQueueSerialOutput;

#endif /* CCB35EB4_6D9A_477B_8D84_B36D42DE88BB */
