#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>


#define DT_DRV_COMPAT our_driver
LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config{
    struct gpio_dt_spec led;
};


static int our_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    const struct our_driver_config *cfg = dev->config;
    LOG_INF("SAMPLE FETCH: LED %d set ON", &cfg->led.pin);
    int ret = gpio_pin_set_dt(&cfg->led, 1);
    return ret;
}

static int our_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct our_driver_config *cfg = dev->config;
    LOG_INF("CHANNEL GET: LED %d set OFF", &cfg->led.pin);
    val->val1 = 0;
    val->val2 = 0;
    int ret = gpio_pin_set_dt(&cfg->led, 0);
    return ret;
}
static DEVICE_API(sensor, api_mackenzie_lecture) = {
    .channel_get = our_channel_get,
    .sample_fetch = our_sample_fetch,

};

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
    DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, &our_driver_config_##inst, \
                          POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY,          \
                          &api_mackenzie_lecture);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE);