#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifiers for the on board leds */
#define LED_NODE_G DT_ALIAS(led0)
#define LED_NODE_O DT_ALIAS(led1)
#define LED_NODE_R DT_ALIAS(led2)

static const struct gpio_dt_spec ledg = GPIO_DT_SPEC_GET(LED_NODE_G, gpios);
static const struct gpio_dt_spec ledo = GPIO_DT_SPEC_GET(LED_NODE_O, gpios);
static const struct gpio_dt_spec ledr = GPIO_DT_SPEC_GET(LED_NODE_R, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;
    int total_state;

    if ((!gpio_is_ready_dt(&ledg)) || (!gpio_is_ready_dt(&ledo)) || 
        (!gpio_is_ready_dt(&ledr)))
        return 0;
    
    if ((gpio_pin_configure_dt(&ledg, GPIO_OUTPUT_ACTIVE) < 0) || 
        (gpio_pin_configure_dt(&ledo, GPIO_OUTPUT_INACTIVE) < 0) ||
        (gpio_pin_configure_dt(&ledr, GPIO_OUTPUT_ACTIVE) < 0))
        return 0;
    

    while (1) {
        if (IS_ENABLED(CONFIG_LED_CUSTOM_PATTERN)) {
            if ((gpio_pin_toggle_dt(&ledg) < 0) || (gpio_pin_toggle_dt(&ledo) < 0)
                || (gpio_pin_toggle_dt(&ledr) < 0))  
                return 0;

            led_state = !led_state;
            total_state = (led_state << 2) | (!led_state << 1) | (led_state);
            LOG_INF("LED state: 0x%x", total_state);
        } else {
            if(gpio_pin_toggle_dt(&ledg) < 0)
                return 0;
            led_state = !led_state;
            LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        }
#ifdef CONFIG_LED_DEBUG_ENABLED
        LOG_INF("LED Params: Brightness=%d, FadeTime=%d, SleepTime=%d, CustomPattern=%d",
            CONFIG_LED_BRIGHTNESS, CONFIG_LED_FADE_DURATION,
            CONFIG_BLINK_TIME_MS, IS_ENABLED(CONFIG_LED_CUSTOM_PATTERN));
#endif
        k_msleep(CONFIG_BLINK_TIME_MS);
    }
    return 0;
}
