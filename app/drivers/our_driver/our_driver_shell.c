#include <stdlib.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_string_conv.h>
#include <our_driver.h>

#define DT_DRV_COMPAT our_driver

// Create list of all our_drivers from the DTB
#define OUR_DRIVER_DEV(inst) DEVICE_DT_INST_GET(inst),
static const struct device *const our_devs[] = {
    DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEV)
};

static const struct device *get_our_dev(const struct shell *sh, const char *name) {
    for (size_t i = 0; i < ARRAY_SIZE(our_devs); i++) {
        if (strcmp(our_devs[i]->name, name) == 0) {
            return our_devs[i];
        }
    }
    shell_error(sh, "Device %s not found", name);
    return NULL;
}

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = get_our_dev(sh, argv[1]);
    if (dev == NULL) {
        return -ENODEV;
    }
    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        shell_error(sh, "sensor_sample_fetch failed: %d", ret);
        return ret;
    }
    shell_print(sh, "%s: sample fetched", dev->name);
    return 0;
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = get_our_dev(sh, argv[1]);
    if (dev == NULL) {
        return -ENODEV;
    }

    enum sensor_channel chan = SENSOR_CHAN_ALL;

    struct sensor_value val;
    int ret = sensor_channel_get(dev, chan, &val);
    if (ret < 0) {
        shell_error(sh, "sensor_channel_get failed: %d", ret);
        return ret;
    }
    shell_print(sh, "%s: channel %d = %d.%06d", dev->name, chan, val.val1, abs(val.val2));
    return 0;
}

static int cmd_info(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = get_our_dev(sh, argv[1]);
    if (dev == NULL) {
        return -ENODEV;
    }
    shell_print(sh, "name: %s", dev->name);
    shell_print(sh, "ready: %s", device_is_ready(dev) ? "yes" : "no");
    return 0;
}

static int cmd_led(const struct shell *sh, size_t argc, char **argv) {
    const struct device *dev = get_our_dev(sh, argv[1]);
    if (dev == NULL) {
        return -ENODEV;
    }
    if (argc < 3) {
        shell_error(sh, "Missing value\nUsage: led <device> <0|1>");
        return -EINVAL;
    }
    int err = 0;
    long enabled = shell_strtol(argv[2], 10, &err);
    if (err != 0 || enabled < 0 || enabled > 1) {
        shell_error(sh, "Invalid value '%s', must be 0 or 1", argv[2]);
        return -EINVAL;
    }
    int ret = our_driver_set_led_enabled(dev, enabled == 1);
    if (ret < 0) {
        shell_error(sh, "our_driver_set_led_enabled failed: %d", ret);
        return ret;
    }
    shell_print(sh, "%s: LED %s", dev->name, enabled ? "enabled" : "disabled");
    return 0;
}

// Tab completion for our_driver targets
static void device_name_get(size_t idx, struct shell_static_entry *entry) {
    entry->syntax = (idx < ARRAY_SIZE(our_devs)) ? our_devs[idx]->name : NULL;
    entry->handler = NULL;
    entry->help = NULL;
    entry->subcmd = NULL;
}
SHELL_DYNAMIC_CMD_CREATE(dsub_device_name, device_name_get);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_our_driver,
    SHELL_CMD_ARG(fetch, &dsub_device_name, "Fetch a sample\nUsage: fetch <device>", cmd_fetch, 2, 0),
    SHELL_CMD_ARG(read, &dsub_device_name, "Read a channel\nUsage: read <device>", cmd_read, 2, 0),
    SHELL_CMD_ARG(info, &dsub_device_name, "Print device name and ready state\nUsage: info <device>", cmd_info, 2, 0),
    SHELL_CMD_ARG(led, &dsub_device_name, "Enable/disable LED on fetch\nUsage: led <device> <0|1>", cmd_led, 2, 1),
    SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(sensor, &sub_our_driver, "Our driver commands", NULL);
