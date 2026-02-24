/*
 * Example program to use deep_sleep functionality with elrebo-de/deep_sleep
 */

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "deep_sleep.hpp"
RTC_DATA_ATTR int bootCount = 0;

#include "esp_log.h"
#include "sdkconfig.h"

static const char *tag = "deep sleep test";

extern "C" void app_main(void)
{
    // short delay to reconnect logging
    vTaskDelay(pdMS_TO_TICKS(5000)); // delay 0.5 seconds

    ESP_LOGI(tag, "Example Program");

    /* Initialize DeepSleep class */
    ESP_LOGI(tag, "DeepSleep");
    DeepSleep deepSleep(
		std::string("DeepSleep"), // tag
		&bootCount // Address of int bootCount in RTC_DATA
    );

    ESP_LOGI(tag, "GetWakeupReason");
    esp_sleep_wakeup_cause_t wakeupReason = deepSleep.GetWakeupReason();

    switch(wakeupReason) {
        case ESP_SLEEP_WAKEUP_EXT0 : ESP_LOGI(tag, "Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : ESP_LOGI(tag, "Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : ESP_LOGI(tag, "Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : ESP_LOGI(tag, "Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : ESP_LOGI(tag, "Wakeup caused by ULP program"); break;
        case ESP_SLEEP_WAKEUP_GPIO : ESP_LOGI(tag, "Wakeup caused by gpio"); break;
        default : ESP_LOGI(tag, "Wakeup was not caused by deep sleep: %d",wakeupReason); break;
    }

    ESP_LOGI(tag, "waiting for 5 seconds ...");
    vTaskDelay(pdMS_TO_TICKS(5000)); // delay 5 seconds

    bool rc = false;

    ESP_LOGI(tag, "EnableTimerWakeup");
    ESP_ERROR_CHECK(deepSleep.EnableTimerWakeup(30, "sec"));  // enable wake up after 30 seconds sleep time

    ESP_LOGI(tag, "EnableGpioWakeup");
    #if defined(CONFIG_IDF_TARGET_ESP32C3)
        ESP_ERROR_CHECK(deepSleep.EnableGpioWakeup((gpio_num_t) 2, 1));  // enable wake up when GPIO 2 is pulled up
    #elif defined(CONFIG_IDF_TARGET_ESP32)
        ESP_ERROR_CHECK(deepSleep.EnableGpioWakeup((gpio_num_t) 39, 0));  // enable wake up when GPIO 39 is pulled down
    #elif defined(CONFIG_IDF_TARGET_ESP32C6)
        ESP_ERROR_CHECK(deepSleep.EnableGpioWakeup((gpio_num_t) 2, 1));  // enable wake up when GPIO 2 is pulled up
    #endif

    ESP_LOGI(tag, "GoToDeepSleep");
    rc = deepSleep.GoToDeepSleep(); // go to deep sleep

    // this statement will not be reached, if GoToDeepSleep is working
    ESP_LOGI(tag, "GoToDeepSleep rc=%u", rc);
}
