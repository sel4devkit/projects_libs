/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2013 Google, Inc
 *
 * (C) Copyright 2012
 * Pavel Herrmann <morpheus.ibis@gmail.com>
 * Marek Vasut <marex@denx.de>
 */

#ifndef _DM_DEVICE_INTERNAL_H
#define _DM_DEVICE_INTERNAL_H

#include <linker_lists.h>
#include <dm/ofnode.h>

struct device_node;
struct udevice;

/*
 * These two macros DM_DEVICE_INST and DM_DEVICE_REF are only allowed in code
 * generated by dtoc, because the ordering is important and if other instances
 * creep in then they may mess up the ordering expected by dtoc.
 *
 * It is OK to use them with 'extern' though, since that does not actually
 * add a new record to the linker_list.
 */

/**
 * DM_DEVICE_INST() - Declare a bound device ready for run-time use
 *
 * This adds an actual struct udevice to a list which is found by driver model
 * on start-up.
 *
 * For example:
 *
 * extern U_BOOT_DRIVER(sandbox_fixed_clock);
 * extern DM_UCLASS_INST(clk);
 *
 * DM_DEVICE_INST(clk_fixed) = {
 *	.driver		= DM_DRIVER_REF(sandbox_fixed_clock),
 *	.name		= "sandbox_fixed_clock",
 *	.plat_		= &_sandbox_fixed_clock_plat_clk_fixed,
 *	.uclass		= DM_UCLASS_REF(clk),
 *	...
 *	.seq_		= 0,
 * };
 *
 * @_name: Name of the udevice. This must be a valid C identifier, used by the
 *	linker_list.
 */
#define DM_DEVICE_INST(_name)						\
	ll_entry_declare(struct udevice, _name, udevice)

/**
 * DM_DEVICE_REF() - Get a reference to a device
 *
 * This is useful in data structures and code for referencing a udevice at
 * build time. Before this is used, an extern DM_DEVICE_INST() must have been
 * declared.
 *
 * For example:
 *
 * extern DM_DEVICE_INST(clk_fixed);
 *
 * struct udevice *devs[] = {
 *	DM_DEVICE_REF(clk_fixed),
 * };
 *
 * @_name: Name of the udevice. This must be a valid C identifier, used by the
 *	linker_list
 * @returns struct udevice * for the device
 */
#define DM_DEVICE_REF(_name)						\
	ll_entry_ref(struct udevice, _name, udevice)

/**
 * DM_DEVICE_GET() - Get a pointer to a given device
 *
 * This is similar to DM_DEVICE_REF() except that it does not need the extern
 * declaration before it. However it cannot be used in a data structures, only
 * in code within a function.
 *
 * For example:
 *
 * void some_function() {
 *	struct udevice *dev = DM_DEVICE_GET(clk_fixed);
 * ...
 * }
 */
#define DM_DEVICE_GET(__name)						\
	ll_entry_get(struct udevice, __name, udevice)

/**
 * device_bind() - Create a device and bind it to a driver
 *
 * Called to set up a new device attached to a driver. The device will either
 * have plat, or a device tree node which can be used to create the
 * plat.
 *
 * Once bound a device exists but is not yet active until device_probe() is
 * called.
 *
 * @parent: Pointer to device's parent, under which this driver will exist
 * @drv: Device's driver
 * @name: Name of device (e.g. device tree node name)
 * @plat: Pointer to data for this device - the structure is device-
 * specific but may include the device's I/O address, etc.. This is NULL for
 * devices which use device tree.
 * @ofnode: Devicetree node for this device. This is ofnode_null() for
 * devices which don't use devicetree or don't have a node.
 * @devp: if non-NULL, returns a pointer to the bound device
 * Return: 0 if OK, -ve on error
 */
int device_bind(struct udevice *parent, const struct driver *drv,
		const char *name, void *plat, ofnode node,
		struct udevice **devp);

/**
 * device_bind_with_driver_data() - Create a device and bind it to a driver
 *
 * Called to set up a new device attached to a driver, in the case where the
 * driver was matched to the device by means of a match table that provides
 * driver_data.
 *
 * Once bound a device exists but is not yet active until device_probe() is
 * called.
 *
 * @parent: Pointer to device's parent, under which this driver will exist
 * @drv: Device's driver
 * @name: Name of device (e.g. device tree node name)
 * @driver_data: The driver_data field from the driver's match table.
 * @node: Device tree node for this device. This is invalid for devices which
 * don't use device tree.
 * @devp: if non-NULL, returns a pointer to the bound device
 * Return: 0 if OK, -ve on error
 */
int device_bind_with_driver_data(struct udevice *parent,
				 const struct driver *drv, const char *name,
				 ulong driver_data, ofnode node,
				 struct udevice **devp);
/**
 * device_bind_by_name: Create a device and bind it to a driver
 *
 * This is a helper function used to bind devices which do not use device
 * tree.
 *
 * @parent: Pointer to device's parent
 * @pre_reloc_only: If true, bind the driver only if its DM_FLAG_PRE_RELOC flag
 * is set. If false bind the driver always.
 * @info: Name and plat for this device
 * @devp: if non-NULL, returns a pointer to the bound device
 * Return: 0 if OK, -ve on error
 */
int device_bind_by_name(struct udevice *parent, bool pre_reloc_only,
			const struct driver_info *info, struct udevice **devp);

/**
 * device_reparent: reparent the device to a new parent
 *
 * @dev: pointer to device to be reparented
 * @new_parent: pointer to new parent device
 * Return: 0 if OK, -ve on error
 */
int device_reparent(struct udevice *dev, struct udevice *new_parent);

/**
 * device_of_to_plat() - Read platform data for a device
 *
 * Read platform data for a device (typically from the device tree) so that
 * the information needed to probe the device is present.
 *
 * This may cause some others devices to be probed if this one depends on them,
 * e.g. a GPIO line will cause a GPIO device to be probed.
 *
 * All private data associated with the device is allocated.
 *
 * @dev: Pointer to device to process
 * Return: 0 if OK, -ve on error
 */
int device_of_to_plat(struct udevice *dev);

/**
 * device_probe() - Probe a device, activating it
 *
 * Activate a device so that it is ready for use. All its parents are probed
 * first.
 *
 * @dev: Pointer to device to probe
 * Return: 0 if OK, -ve on error
 */
int device_probe(struct udevice *dev);

/**
 * device_remove() - Remove a device, de-activating it
 *
 * De-activate a device so that it is no longer ready for use. All its
 * children are deactivated first.
 *
 * @dev: Pointer to device to remove
 * @flags: Flags for selective device removal (DM_REMOVE_...)
 * Return: 0 if OK, -EKEYREJECTED if not removed due to flags, -EPROBE_DEFER if
 *	this is a vital device and flags is DM_REMOVE_NON_VITAL, other -ve on
 *	error (such an error here is normally a very bad thing)
 */
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
int device_remove(struct udevice *dev, uint flags);
#else
static inline int device_remove(struct udevice *dev, uint flags) { return 0; }
#endif

/**
 * device_unbind() - Unbind a device, destroying it
 *
 * Unbind a device and remove all memory used by it
 *
 * @dev: Pointer to device to unbind
 * Return: 0 if OK, -ve on error
 */
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
int device_unbind(struct udevice *dev);
#else
static inline int device_unbind(struct udevice *dev) { return 0; }
#endif

#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
void device_free(struct udevice *dev);
#else
static inline void device_free(struct udevice *dev) {}
#endif

/**
 * device_chld_unbind() - Unbind all device's children from the device if bound
 *			  to drv
 *
 * On error, the function continues to unbind all children, and reports the
 * first error.
 *
 * @dev:	The device that is to be stripped of its children
 * @drv:	The targeted driver
 * Return: 0 on success, -ve on error
 */
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
int device_chld_unbind(struct udevice *dev, struct driver *drv);
#else
static inline int device_chld_unbind(struct udevice *dev, struct driver *drv)
{
	return 0;
}
#endif

/**
 * device_chld_remove() - Stop all device's children
 *
 * This continues through all children recursively stopping part-way through if
 * an error occurs. Return values of -EKEYREJECTED are ignored and processing
 * continues, since they just indicate that the child did not elect to be
 * removed based on the value of @flags. Return values of -EPROBE_DEFER cause
 * processing of other children to continue, but the function will return
 * -EPROBE_DEFER.
 *
 * @dev:	The device whose children are to be removed
 * @drv:	The targeted driver
 * @flags:	Flag, if this functions is called in the pre-OS stage
 * Return: 0 on success, -EPROBE_DEFER if any child failed to remove, other
 *	-ve on error
 */
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
int device_chld_remove(struct udevice *dev, struct driver *drv,
		       uint flags);
#else
static inline int device_chld_remove(struct udevice *dev, struct driver *drv,
				     uint flags)
{
	return 0;
}
#endif

/**
 * dev_set_priv() - Set the private data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * private data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev		Device to check
 * @priv	New private-data pointer
 */
void dev_set_priv(struct udevice *dev, void *priv);

/**
 * dev_set_parent_priv() - Set the parent-private data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * parent-private data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev:	Device to update
 * @parent_priv: New parent-private data
 */
void dev_set_parent_priv(struct udevice *dev, void *parent_priv);

/**
 * dev_set_uclass_priv() - Set the uclass private data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * uclass-private data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev:	Device to update
 * @uclass_priv: New uclass private data
 */
void dev_set_uclass_priv(struct udevice *dev, void *uclass_priv);

/**
 * dev_set_plat() - Set the platform data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * platform data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev		Device to check
 * @plat	New platform-data pointer
 */
void dev_set_plat(struct udevice *dev, void *priv);

/**
 * dev_set_parent_plat() - Set the parent platform data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * parent platform data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev:	Device to update
 * @parent_plat: New parent platform data
 */
void dev_set_parent_plat(struct udevice *dev, void *parent_plat);

/**
 * dev_set_uclass_plat() - Set the uclass platform data for a device
 *
 * This is normally handled by driver model, which automatically allocates
 * uclass platform data when an 'auto' size if provided by the driver.
 *
 * Use this function to override normal operation for special situations, such
 * as needing to allocate a variable amount of data.
 *
 * If OF_PLATDATA_RT is enabled, this function cannot be used out of core driver
 * model code, since the pointer must be within the gd->dm_priv_base region.
 *
 * @dev:	Device to update
 * @uclass_plat: New uclass platform data
 */
void dev_set_uclass_plat(struct udevice *dev, void *uclass_plat);

/**
 * simple_bus_translate() - translate a bus address to a system address
 *
 * This handles the 'ranges' property in a simple bus. It translates the
 * device address @addr to a system address using this property.
 *
 * @dev:	Simple bus device (parent of target device)
 * @addr:	Address to translate
 * Return: new address
 */
fdt_addr_t simple_bus_translate(struct udevice *dev, fdt_addr_t addr);

/* Cast away any volatile pointer */
#define DM_ROOT_NON_CONST		(((gd_t *)gd)->dm_root)
#define DM_UCLASS_ROOT_NON_CONST	(((gd_t *)gd)->uclass_root)
#define DM_UCLASS_ROOT_S_NON_CONST	(((gd_t *)gd)->uclass_root_s)

/* device resource management */
#ifdef CONFIG_DEVRES

/**
 * devres_release_probe - Release managed resources allocated after probing
 * @dev: Device to release resources for
 *
 * Release all resources allocated for @dev when it was probed or later.
 * This function is called on driver removal.
 */
void devres_release_probe(struct udevice *dev);

/**
 * devres_release_all - Release all managed resources
 * @dev: Device to release resources for
 *
 * Release all resources associated with @dev.  This function is
 * called on driver unbinding.
 */
void devres_release_all(struct udevice *dev);

#else /* ! CONFIG_DEVRES */

static inline void devres_release_probe(struct udevice *dev)
{
}

static inline void devres_release_all(struct udevice *dev)
{
}

#endif /* ! CONFIG_DEVRES */
#endif