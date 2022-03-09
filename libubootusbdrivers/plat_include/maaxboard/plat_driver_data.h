// Define the number of different driver elements to be used on this platform.
#define DEFINE_UBOOT_UCLASS_DRIVER_COUNT        3
#define DEFINE_UBOOT_DRIVER_COUNT               2
#define DEFINE_UBOOT_USB_DRIVER_ENTRY_COUNT     0
#define DEFINE_UBOOT_DRIVER_INFO_COUNT          0
#define DEFINE_UBOOT_UDEVICE_COUNT              0

// Define the uclass drivers to be used on this platform.
extern struct uclass_driver _u_boot_uclass_driver__root;
extern struct uclass_driver _u_boot_uclass_driver__usb;
extern struct uclass_driver _u_boot_uclass_driver__usb_hub;

// Define the drivers to be used on this platform.
extern struct driver _u_boot_driver__root_driver;
extern struct driver _u_boot_driver__dwc3_generic_host;
