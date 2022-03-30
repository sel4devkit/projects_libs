// Define the number of different driver elements to be used on this platform.
#define DEFINE_UBOOT_UCLASS_DRIVER_COUNT        9
#define DEFINE_UBOOT_DRIVER_COUNT               9
#define DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT     2
#define DEFINE_UBOOT_DRIVER_INFO_COUNT          0
#define DEFINE_UBOOT_UDEVICE_COUNT              0
#define DEFINE_UBOOT_CMD_COUNT                  2

// Define the uclass drivers to be used on this platform.
extern struct uclass_driver _u_boot_uclass_driver__nop;
extern struct uclass_driver _u_boot_uclass_driver__root;
extern struct uclass_driver _u_boot_uclass_driver__simple_bus;
extern struct uclass_driver _u_boot_uclass_driver__usb;
extern struct uclass_driver _u_boot_uclass_driver__usb_hub;
extern struct uclass_driver _u_boot_uclass_driver__usb_dev_generic;
extern struct uclass_driver _u_boot_uclass_driver__phy;
extern struct uclass_driver _u_boot_uclass_driver__usb_mass_storage;
extern struct uclass_driver _u_boot_uclass_driver__blk;

// Define the drivers to be used on this platform.
extern struct driver _u_boot_driver__root_driver;
extern struct driver _u_boot_driver__simple_bus;
extern struct driver _u_boot_driver__usb_dev_generic_drv;
extern struct driver _u_boot_driver__usb_generic_hub;
extern struct driver _u_boot_driver__dwc3_generic_wrapper;
extern struct driver _u_boot_driver__dwc3_generic_host;
extern struct driver _u_boot_driver__nxp_imx8mq_usb_phy;
extern struct driver _u_boot_driver__usb_mass_storage;
extern struct driver _u_boot_driver__usb_storage_blk;

// Define the driver entries to be used on this platform.
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_generic_hub;
extern struct usb_driver_entry _u_boot_usb_driver_entry__usb_mass_storage;

// Define the u-boot commands to be used on this platform.
extern struct cmd_tbl _u_boot_cmd__dm;
extern struct cmd_tbl _u_boot_cmd__usb;