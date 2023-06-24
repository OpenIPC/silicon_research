#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/version.h>
#include "hi_osal.h"
#include "hi_osal_init.h"

extern void osal_proc_init(void);
extern void osal_proc_exit(void);
extern int himedia_init(void);
extern void himedia_exit(void);
extern int media_mem_init(void);
extern void media_mem_exit(void);
extern int mem_check_module_param(void);
extern int hi_user_init(void);
extern void hi_user_exit(void);

int osal_init(void)
{
    osal_proc_init();
    hi_user_init();
    himedia_init();
    media_mem_init();
    osal_printk("hi_osal %s init success!\n", HI_OSAL_VERSION);
    return 0;
}

void osal_exit(void)
{
	osal_proc_exit();
	himedia_exit();
    hi_user_exit();
    media_mem_exit();
	osal_printk("hi_osal v1.0 exit!\n");
}


#define MMZ_SETUP_CMDLINE_LEN 	256
#define MMZ_ALLOCATOR_NAME_LEN	32

extern int anony;
extern char setup_zones[MMZ_SETUP_CMDLINE_LEN];
extern char setup_allocator[MMZ_ALLOCATOR_NAME_LEN];

static int osal_probe(struct platform_device *pdev)
{
	int ret;

	osal_platform_get_module_param(pdev, "anony", int, &anony);
	osal_platform_get_modparam_string(pdev, "setup_zones", MMZ_SETUP_CMDLINE_LEN, setup_zones);
	//osal_platform_get_modparam_string(pdev, "setup_allocator", MMZ_ALLOCATOR_NAME_LEN, setup_allocator);
	ret = osal_init();
	if (ret)
	{
		return ret;
	}
	
    return 0;
}

static int osal_remove(struct platform_device *pdev)
{
	osal_exit();
    return 0;
}

static const struct of_device_id osal_match[] __maybe_unused = {
        { .compatible = "hisilicon,osal" },
        {},
};
static struct platform_driver osal_driver = {
        .probe          = osal_probe,
        .remove         = osal_remove,
        .driver         = {
                .name   = "osal",
                .of_match_table = of_match_ptr(osal_match),
        },
};

static int __init osal_mod_init(void)
{
    int ret;
    mem_check_module_param();
    ret = platform_driver_register(&osal_driver);
    if (ret) {
        platform_driver_unregister(&osal_driver);
        osal_printk("hi_osal driver register failed!\n");
        return ret;
    }

    return ret;
}

static void __exit osal_mod_exit(void)
{
    platform_driver_unregister(&osal_driver);
}
module_init(osal_mod_init);
module_exit(osal_mod_exit);

MODULE_AUTHOR("Hisilicon");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
