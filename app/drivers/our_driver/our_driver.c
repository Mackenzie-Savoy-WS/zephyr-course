#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <our_driver.h>


#define DT_DRV_COMPAT our_driver
LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config{
    struct gpio_dt_spec led;
};

struct our_driver_data{
    bool led_enabled;
};


static int our_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    const struct our_driver_config *cfg = dev->config;
    struct our_driver_data *data = dev->data;
    LOG_INF("SAMPLE FETCH: LED %d set %s", cfg->led.pin, data->led_enabled ? "ON" : "OFF (disabled)");
    int ret = gpio_pin_set_dt(&cfg->led, data->led_enabled ? 1 : 0);
    return ret;
}

static int our_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct our_driver_config *cfg = dev->config;
    LOG_INF("CHANNEL GET: LED %d set OFF", cfg->led.pin);
    val->val1 = 0;
    val->val2 = 0;
    int ret = gpio_pin_set_dt(&cfg->led, 0);
    return ret;
}
static DEVICE_API(sensor, api_mackenzie_lecture) = {
    .channel_get = our_channel_get,
    .sample_fetch = our_sample_fetch,

};

// val to enable or disable the LED
int our_driver_set_led_enabled(const struct device *dev, bool enabled) {
    if (dev == NULL) {
        return -EINVAL;
    }
    struct our_driver_data *data = dev->data;
    data->led_enabled = enabled;
    LOG_INF("LED %s", enabled ? "enabled" : "disabled");
    return 0;
}

// Define inst
static int init(const struct device *dev) {
    const struct our_driver_config *cfg = dev->config;
    if(!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("LED device not ready");
        return -ENODEV;
    }
    int ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_ACTIVE);
    return ret;
}

#define OUR_DRIVER_DEFINE(inst)                                              \
    static const struct our_driver_config our_driver_config_##inst = {      \
        .led = GPIO_DT_SPEC_GET(DT_INST_PHANDLE(inst, led), gpios),          \
    };                                                                       \
    static struct our_driver_data our_driver_data_##inst = {                \
        .led_enabled = true,                                                 \
    };                                                                       \
    DEVICE_DT_INST_DEFINE(inst, init, NULL, &our_driver_data_##inst,         \
                          &our_driver_config_##inst,                         \
                          POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,          \
                          &api_mackenzie_lecture);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE);