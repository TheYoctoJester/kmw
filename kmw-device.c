#include <linux/module.h>
#include <linux/platform_device.h>

#include "kmw.h"

static unsigned int kmw_dev_led_red = 7;
static unsigned int kmw_dev_led_green = 50;
static unsigned int kmw_dev_led_blue = 51;

module_param(kmw_dev_led_red, uint, 0644);
module_param(kmw_dev_led_green, uint, 0644);
module_param(kmw_dev_led_blue, uint, 0644);

static unsigned int * const kmw_dev_leds[] = {
	&kmw_dev_led_red,
	&kmw_dev_led_green,
	&kmw_dev_led_blue,
};

#define KMW_DEV_LED_COUNT (sizeof(kmw_dev_leds)/sizeof(unsigned int *))

void    kmw_release(struct device *dev)
{
	PDEBUG("device release\n");
}

void	kmw_set_output(unsigned int value)
{
	int i;
	for (i = 0; i < KMW_DEV_LED_COUNT; ++i)
		PDEBUG("device - set led%d @ gpio %u to %s\n", i, *kmw_dev_leds[i], ((value & (1L << i)) ? "high" : "low"));
}

static struct kmw_ops kmw_device_ops = {
	.set_output = kmw_set_output,
};

static struct platform_device kmw_device = {
	.name = KMWNAME,
	.dev = {
		.release = kmw_release,
		.platform_data = &kmw_device_ops,
	}
};

static int kmw_device_init(void)
{
	int err;

	PDEBUG("device_init\n");
	err = platform_device_register(&kmw_device);
	if (err < 0)
		goto err1;
	return 0;
err1:
	return err;
}

static void kmw_device_exit(void)
{
	PDEBUG("device_exit\n");
	platform_device_unregister(&kmw_device);
}

module_init(kmw_device_init);
module_exit(kmw_device_exit);

MODULE_LICENSE("GPL");