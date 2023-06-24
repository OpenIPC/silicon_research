#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/version.h>
#include <linux/pinctrl/consumer.h>

int osal_platform_get_modparam_uint(void *pdev, const char *name, unsigned int *value)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
	return 0;
#else
	struct device_node *np = ((struct platform_device *)pdev)->dev.of_node;
	unsigned int temp_value;

	if (of_property_read_u32(np, name, &temp_value)){
		return -1;
	}
	*value = temp_value;
	return 0;
#endif
}
EXPORT_SYMBOL(osal_platform_get_modparam_uint);

int osal_platform_get_modparam_int(void *pdev, const char *name, int *value)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
	return 0;
#else
	struct device_node *np = ((struct platform_device *)pdev)->dev.of_node;
	unsigned int temp_value;
	
	if (of_property_read_u32(np, name, &temp_value)){
		return -1;
	}
	*value = (int)temp_value;
	return 0;
#endif
}
EXPORT_SYMBOL(osal_platform_get_modparam_int);

int osal_platform_get_modparam_charp(void *pdev, const char *name, char *value)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
	return 0;
#else
	struct device_node *np = ((struct platform_device *)pdev)->dev.of_node;
	const char *string = NULL;
	
	if (of_property_read_string(np, name, &string)){
		return -1;
	}

	if (NULL == value){
		printk("%s: module parameter %s is null pointer!!\n", __func__, name);
		return -1;
	}
	
	strcpy(value, string);
	return 0;
#endif
}
EXPORT_SYMBOL(osal_platform_get_modparam_charp);

int osal_platform_get_modparam_string(void *pdev, const char *name, unsigned int size, char *value)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,19,0)
	return 0;
#else
	struct device_node *np = ((struct platform_device *)pdev)->dev.of_node;
	const char *string = NULL;
	
	if (of_property_read_string(np, name, &string)){
		return -1;
	}
	strncpy(value, string, size);
	return 0;
#endif
}
EXPORT_SYMBOL(osal_platform_get_modparam_string);


int osal_of_property_read_u32(void *np, const char *propname, unsigned int *value)
{
	return of_property_read_u32((struct device_node *)np, propname, value);
}
EXPORT_SYMBOL(osal_of_property_read_u32);


void* osal_devm_pinctrl_get(void *dev)
{
	return devm_pinctrl_get((struct device *)dev);
}
EXPORT_SYMBOL(osal_devm_pinctrl_get);

void osal_devm_pinctrl_put(void *p)
{
	devm_pinctrl_put((struct pinctrl *)p);
}
EXPORT_SYMBOL(osal_devm_pinctrl_put);

void* osal_pinctrl_lookup_state(void *p, const char *name)
{
	return pinctrl_lookup_state((struct pinctrl *)p, name);
}
EXPORT_SYMBOL(osal_pinctrl_lookup_state);

void osal_pinctrl_get_select(void *dev, const char *name)
{
	pinctrl_get_select((struct device *)dev, name);
}
EXPORT_SYMBOL(osal_pinctrl_get_select);

int osal_pinctrl_pm_select_default_state(void *dev)
{
	return pinctrl_pm_select_default_state((struct device *)dev);
}
EXPORT_SYMBOL(osal_pinctrl_pm_select_default_state);

int osal_pinctrl_select_state(void *p, void *state)
{
	return pinctrl_select_state((struct pinctrl *)p, (struct pinctrl_state *)state);
}
EXPORT_SYMBOL(osal_pinctrl_select_state);

int osal_clk_prepare_enable(void* clk)
{
	return clk_prepare_enable((struct clk *)clk);
}
EXPORT_SYMBOL(osal_clk_prepare_enable);

void osal_clk_disable_unprepare(void *clk)
{
	clk_disable_unprepare((struct clk *)clk);
}
EXPORT_SYMBOL(osal_clk_disable_unprepare);

unsigned int osal_clk_get_rate(void *clk)
{
	return clk_get_rate((struct clk *)clk);
}
EXPORT_SYMBOL(osal_clk_get_rate);

int osal_clk_set_rate(void *clk, unsigned int clk_freq)
{
	return clk_set_rate((struct clk *)clk, clk_freq);
}
EXPORT_SYMBOL(osal_clk_set_rate);

void* osal_devm_kzalloc(void *dev, unsigned int size)
{
	return devm_kzalloc((struct device *)dev, size, GFP_KERNEL);
}
EXPORT_SYMBOL(osal_devm_kzalloc);

int osal_platform_driver_register(void *drv)
{
    return __platform_driver_register((struct platform_driver *)drv, THIS_MODULE);
}
EXPORT_SYMBOL(osal_platform_driver_register);

void osal_platform_driver_unregister(void *drv)
{
    return platform_driver_unregister((struct platform_driver *)drv);
}
EXPORT_SYMBOL(osal_platform_driver_unregister);

void* osal_platform_get_resource_byname(void *dev, unsigned int type, 
    const char *name)
{
    return (void*)platform_get_resource_byname((struct platform_device *)dev, type, name);
}
EXPORT_SYMBOL(osal_platform_get_resource_byname);

void* osal_platform_get_resource(void *dev, unsigned int type, unsigned int num)
{
    return (void*)platform_get_resource((struct platform_device *)dev, type, num);
}
EXPORT_SYMBOL(osal_platform_get_resource);

int osal_platform_get_irq(void *dev, unsigned int num)
{
    return platform_get_irq((struct platform_device *)dev, num);
}
EXPORT_SYMBOL(osal_platform_get_irq);

int osal_platform_get_irq_byname(void *dev, const char *name)
{
    return platform_get_irq_byname((struct platform_device *)dev, name);
}
EXPORT_SYMBOL(osal_platform_get_irq_byname);
