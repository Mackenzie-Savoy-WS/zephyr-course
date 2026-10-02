#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifiers for the on board leds */
#define LED_NODE_G DT_ALIAS(app_led)


static const struct gpio_dt_spec ledg = GPIO_DT_SPEC_GET(LED_NODE_G, gpios);


LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    int total_state;

    if ((!gpio_is_ready_dt(&ledg)))
        return 0;
    
    if ((gpio_pin_configure_dt(&ledg, GPIO_OUTPUT_ACTIVE) < 0))
        return 0;
    

    while (1) {
    
        if ((gpio_pin_toggle_dt(&ledg) < 0))  
            return 0;

        led_state = !led_state;
        
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
