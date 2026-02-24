# Deep sleep component

ESP-IDF component for the deep sleep function. It runs on
any ESP32 processor and is built using the ESP-IDF build system in version 5.5+.

The component is implemented as C++ class `DeepSleep`.

## Connecting the component

The constructor of class `DeepSleep` has two parameters:

| Parameter | Type of Parameter | Usage                                        |
|:----------|:------------------|:---------------------------------------------|
| tag       | std::string       | the tag to be used in the ESP log            |
| bootCount | int *             | pointer to the bootCount in RTC_DATA storage |

# Usage
To use the DeepSleep component you have to include "deep_sleep.hpp" and 
to define an int bootCount as RTC_DATA_ATTR.

The wakeup sources can be any of the following:

* Timer
* GPIO

To enable wakeup by timer the method EnableTimerWakeup is used.

Method EnableTimerWakeup has two parameters:

| Parameter     | Type of Parameter | Usage                                                                            |
|:--------------|:------------------|:---------------------------------------------------------------------------------|
| sleepTime     | unsigned long     | the sleep time                                                                   |
| sleepTimeUnit | std::string       | the unit in which the sleep time is given {"min", "sec", "milliSec", "microSec"} |

To enable wakeup by GPIO the method EnableGpioWakeup is used.

Method EnableGpioWakeup has two parameters:

| Parameter | Type of Parameter | Usage                           |
|:----------|:------------------|:--------------------------------|
| gpio      | gpio_num_t        | the GPIO number                 |
| level     | int               | the wakeup trigger level {1, 0} |

Example code:

```
#include "deep_sleep.hpp"
RTC_DATA_ATTR int bootCount = 0;

extern "C" void app_main(void)
{
    DeepSleep deepSleep(
		std::string("DeepSleep"), // tag
		&bootCount // Address of int bootCount in RTC_DATA
    );
    
    ESP_LOGI(tag, "GetWakeupReason");
    esp_sleep_wakeup_cause_t wakeupReason = deepSleep.GetWakeupReason(); 
    
    switch(wakeupReason) {
        case ESP_SLEEP_WAKEUP_EXT0 : ESP_LOGI(tag.c_str(), "Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : ESP_LOGI(tag.c_str(), "Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : ESP_LOGI(tag.c_str(), "Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : ESP_LOGI(tag.c_str(), "Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : ESP_LOGI(tag.c_str(), "Wakeup caused by ULP program"); break;
        case ESP_SLEEP_WAKEUP_GPIO : ESP_LOGI(tag.c_str(), "Wakeup caused by gpio"); break;
        default : ESP_LOGI(tag.c_str(), "Wakeup was not caused by deep sleep: %d",wakeupReason); break;
    }

    ...
    
    ESP_LOGI(tag, "EnableTimerWakeup");
    ESP_ERROR_CHECK(deepSleep.EnableTimerWakeup(30, "min"));  // enable wake up after 30 minutes sleep time

    ESP_LOGI(tag, "EnableGpioWakeup");
    ESP_ERROR_CHECK(deepSleep.EnableGpioWakeup((gpio_num_t) 2, 1));  // enable wake up when GPIO 2 is pulled up
    
    ESP_LOGI(tag, "GoToDeepSleep");
    rc = deepSleep.GoToDeepSleep(); // go to deep sleep
    
    // this statement will not be reached, if GoToDeepSleep is working
    ESP_LOGI(tag, "GoToDeepSleep rc=%u", rc);
}
```

## API
The API of the component is located in the include directory ```include/deep_sleep.hpp``` and defines the
C++ class ```DeepSleep```

```C++
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

    esp_err_t EnableTimerWakeup( unsigned long sleepTime,
                                 std::string sleepTimeUnit // {"min", "sec", "milliSec", "microSec"}
                               );
    esp_err_t EnableGpioWakeup( gpio_num_t gpio,
                                int level  // level: 1 = High, 0 = Low
                              );
	esp_err_t GoToDeepSleep();

private:
    std::string tag = "DeepSleep";
    esp_sleep_wakeup_cause_t wakeup_reason; // the wakeup reason for the last boot
};
```

# License
This component is provided under the Apache 2.0 license.
