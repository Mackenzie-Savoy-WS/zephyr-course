#include <zephyr/kernel.h>
#include <zephyr/platform/hooks.h>

void board_early_init_hook(void)
{
}

void board_late_init_hook(void)
{
	printk("My Board: Board Initialized\n");
}