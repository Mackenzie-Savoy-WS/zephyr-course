#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

/* The devicetree node identifiers for the on board leds */
#define LED_NODE_G DT_ALIAS(app_led)


static const struct gpio_dt_spec ledg = GPIO_DT_SPEC_GET(LED_NODE_G, gpios);


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);


int main(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    struct sensor_value val;
    bool led_state = true;
    int total_state;
    
    // LED green for heartbeat
    if ((!gpio_is_ready_dt(&ledg)))
        return 0;
    
    if ((gpio_pin_configure_dt(&ledg, GPIO_OUTPUT_ACTIVE) < 0))
        return 0;
    
    // LED yellow via driver
    if(!device_is_ready(dev)) {
        LOG_INF("Our driver device not ready");
        return 0;
    }

    while (1) {
    
        if ((gpio_pin_toggle_dt(&ledg) < 0))  
            return 0;

        led_state = !led_state;
        
        LOG_INF("LED Green state: %s", led_state ? "ON" : "OFF");

        if(!led_state)
            sensor_sample_fetch(dev);
        else
            sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
        LOG_INF("LED Yellow state: %s", !led_state ? "ON" : "OFF");
        
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
