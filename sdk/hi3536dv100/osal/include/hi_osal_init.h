#ifndef __HI_OSAL_INIT__
#define __HI_OSAL_INIT__

#define osal_platform_get_module_param(pdev, name, type, value) \
	osal_platform_get_modparam_##type(pdev, name, value)

int osal_platform_get_modparam_uint(void *pdev, const char *name, unsigned int *value);
int osal_platform_get_modparam_int(void *pdev, const char *name, int *value);
int osal_platform_get_modparam_charp(void *pdev, const char *name, char *value);
int osal_platform_get_modparam_string(void *pdev, const char *name, unsigned int size, char *value);

/*of function*/
int osal_of_property_read_u32(void *np, char *propname, unsigned int *value);

/*pinctrl*/
void* osal_devm_pinctrl_get(void *dev);
void osal_devm_pinctrl_put(void *p);
void* osal_pinctrl_lookup_state(void *p, const char *name);
void osal_pinctrl_get_select(void *dev, const char *name);
int osal_pinctrl_pm_select_default_state(void *dev);
int osal_pinctrl_select_state(void *p, void *state);

/*clock function*/
int osal_clk_prepare_enable(void *clk);
void osal_clk_disable_unprepare(void *clk);
unsigned int osal_clk_get_rate(void *clk);
int osal_clk_set_rate(void *clk, unsigned int clk_freq);

void* osal_devm_kzalloc(void *dev, unsigned int size);

int osal_platform_driver_register(void *drv);
void osal_platform_driver_unregister(void *drv);
void* osal_platform_get_resource_byname(void *dev, unsigned int type, 
    const char *name);
void* osal_platform_get_resource(void *dev, unsigned int type, 
    unsigned int num);
int osal_platform_get_irq(void *dev, unsigned int num);
int osal_platform_get_irq_byname(void *dev, const char *name);

#define osal_module_driver(osal_driver, osal_register, osal_unregister, ...) \
static int __init osal_driver##_init(void) \
{ \
        return osal_register(&(osal_driver)); \
} \
module_init(osal_driver##_init); \
static void __exit osal_driver##_exit(void) \
{ \
        osal_unregister(&(osal_driver)); \
} \
module_exit(osal_driver##_exit);

#define osal_module_platform_driver(platform_driver) \
        osal_module_driver(platform_driver, osal_platform_driver_register, \
                        osal_platform_driver_unregister)

#endif                
