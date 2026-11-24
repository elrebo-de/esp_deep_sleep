/*
 * deep_sleep.hpp
 *
 *      Author: christophoberle
 *
 * this work is licenced under the Apache 2.0 licence
 */

#ifndef DEEP_SLEEP_HPP_
#define DEEP_SLEEP_HPP_

#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_sleep.h"
#include "esp_log.h"

#include "driver/gpio.h"
#include "driver/rtc_io.h"
#include "math.h"

/* class DeepSleep
   wrapper for esp deep sleep functionality

   Wakeup sources can be:
   * timer
   * one gpio pin

   Tested with
   * ESP32C3
*/
class DeepSleep {
public:
    // Constructor
	DeepSleep( std::string tag,
	           int *bootCount
	         );
	virtual ~DeepSleep();

	uint32_t GetWakeupCauses();

    esp_err_t EnableTimerWakeup( unsigned long sleepTime,
                            std::string sleepTimeUnit // {"min", "sec", "milliSec", "microSec"}
                          );
    esp_err_t EnableGpioWakeup( gpio_num_t gpio,
                           int level  // level: 1 = High, 0 = Low
                          );
    esp_err_t EnableAdcWakeup( gpio_num_t gpio,
                           int level  // level: 1 = High, 0 = Low
                          );
	esp_err_t GoToDeepSleep();

private:
    std::string tag = "DeepSleep";
    uint32_t wakeup_causes; // the wakeup causes for the last boot
};

#endif /* DEEP_SLEEP_HPP_ */
