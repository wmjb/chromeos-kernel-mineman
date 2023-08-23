// SPDX-License-Identifier: GPL-2.0-only
/*
 * EFI stub implementation that is shared by arm and arm64 architectures.
 * This should be #included by the EFI stub implementation files.
 *
 * Copyright (C) 2013,2014 Linaro Limited
 *     Roy Franz <roy.franz@linaro.org
 * Copyright (C) 2013 Red Hat, Inc.
 *     Mark Salter <msalter@redhat.com>
 */

#include <linux/efi.h>
#include <linux/libfdt.h>
#include <asm/efi.h>

#include "efistub.h"

/*
 * This is the base address at which to start allocating virtual memory ranges
 * for UEFI Runtime Services.
 *
 * For ARM/ARM64:
 * This is in the low TTBR0 range so that we can use
 * any allocation we choose, and eliminate the risk of a conflict after kexec.
 * The value chosen is the largest non-zero power of 2 suitable for this purpose
 * both on 32-bit and 64-bit ARM CPUs, to maximize the likelihood that it can
 * be mapped efficiently.
 * Since 32-bit ARM could potentially execute with a 1G/3G user/kernel split,
 * map everything below 1 GB. (512 MB is a reasonable upper bound for the
 * entire footprint of the UEFI runtime services memory regions)
 *
 * For RISC-V:
 * There is no specific reason for which, this address (512MB) can't be used
 * EFI runtime virtual address for RISC-V. It also helps to use EFI runtime
 * services on both RV32/RV64. Keep the same runtime virtual address for RISC-V
 * as well to minimize the code churn.
 */
#define EFI_RT_VIRTUAL_BASE	SZ_512M
#define EFI_RT_VIRTUAL_SIZE	SZ_512M

#ifdef CONFIG_ARM64
# define EFI_RT_VIRTUAL_LIMIT	DEFAULT_MAP_WINDOW_64
#elif defined(CONFIG_RISCV)
# define EFI_RT_VIRTUAL_LIMIT	TASK_SIZE_MIN
#else
# define EFI_RT_VIRTUAL_LIMIT	TASK_SIZE
#endif

static u64 virtmap_base = EFI_RT_VIRTUAL_BASE;
static bool flat_va_mapping;

const efi_system_table_t *efi_system_table;

static struct screen_info *setup_graphics(void)
{
	efi_guid_t gop_proto = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	efi_status_t status;
	unsigned long size;
	void **gop_handle = NULL;
	struct screen_info *si = NULL;

	size = 0;
	status = efi_bs_call(locate_handle, EFI_LOCATE_BY_PROTOCOL,
			     &gop_proto, NULL, &size, gop_handle);
	if (status == EFI_BUFFER_TOO_SMALL) {
		si = alloc_screen_info();
		if (!si)
			return NULL;
		status = efi_setup_gop(si, &gop_proto, size);
		if (status != EFI_SUCCESS) {
			free_screen_info(si);
			return NULL;
		}
	}
	return si;
}

static void install_memreserve_table(void)
{
	struct linux_efi_memreserve *rsv;
	efi_guid_t memreserve_table_guid = LINUX_EFI_MEMRESERVE_TABLE_GUID;
	efi_status_t status;

	status = efi_bs_call(allocate_pool, EFI_LOADER_DATA, sizeof(*rsv),
			     (void **)&rsv);
	if (status != EFI_SUCCESS) {
		efi_err("Failed to allocate memreserve entry!\n");
		return;
	}

	rsv->next = 0;
	rsv->size = 0;
	atomic_set(&rsv->count, 0);

	status = efi_bs_call(install_configuration_table,
			     &memreserve_table_guid, rsv);
	if (status != EFI_SUCCESS)
		efi_err("Failed to install memreserve config table!\n");
}

static u32 get_supported_rt_services(void)
{
	const efi_rt_properties_table_t *rt_prop_table;
	u32 supported = EFI_RT_SUPPORTED_ALL;

	rt_prop_table = get_efi_config_table(EFI_RT_PROPERTIES_TABLE_GUID);
	if (rt_prop_table)
		supported &= rt_prop_table->runtime_services_supported;

	return supported;
}

/*
 * EFI entry point for the arm/arm64 EFI stubs.  This is the entrypoint
 * that is described in the PE/COFF header.  Most of the code is the same
 * for both archictectures, with the arch-specific code provided in the
 * handle_kernel_image() function.
 */
efi_status_t __efiapi efi_pe_entry(efi_handle_t handle,
				   efi_system_table_t *sys_table_arg)
{
	efi_loaded_image_t *image;
	efi_status_t status;
	unsigned long image_addr;
	unsigned long image_size = 0;
	/* addr/point and size pairs for memory management*/
	unsigned long initrd_addr = 0;
	unsigned long initrd_size = 0;
	unsigned long fdt_addr = 0;  /* Original DTB */
	unsigned long fdt_size = 0;
	char *cmdline_ptr = NULL;
	int cmdline_size = 0;
	efi_guid_t loaded_image_proto = LOADED_IMAGE_PROTOCOL_GUID;
	unsigned long reserve_addr = 0;
	unsigned long reserve_size = 0;
	enum efi_secureboot_mode secure_boot;
	struct screen_info *si;
	efi_properties_table_t *prop_tbl;
	unsigned long max_addr;
	enum winrt_device_names winrt_device = unknown_device;
	int i;

	u8 buf_recv[4];
	u8 buf_send = 0;
	u32 size = 1;
	u8 address = 0x48;
	u8 tmp;

	efi_system_table = sys_table_arg;

	/* Check if we were booted by the EFI firmware */
	if (efi_system_table->hdr.signature != EFI_SYSTEM_TABLE_SIGNATURE) {
		status = EFI_INVALID_PARAMETER;
		goto fail;
	}

	status = check_platform_features();
	if (status != EFI_SUCCESS)
		goto fail;

/*
	for (i = 0; i < 256; i++) {
		if (i % 16 == 0) {
			efi_printk("\n");
			efi_printk("%02x: ", i);
		}

		i2c_recv_buf_small(buf_recv, size, 0, address, i);
		efi_printk("%02x ", buf_recv[0]);
	}

	while(true) {
		i2c_recv_buf_small(buf_recv, size, 0, address, buf_send);
		if (tmp != buf_recv[0]) {
			tmp = buf_recv[0];
			efi_info("first byte: %04x\n", tmp);
		}
	}
*/
	uint8_t *base = (uint8_t*) 0x6000d000;

	/*
	 * Figure out on which Windows RT device is booted.
	 */
	winrt_device = winrt_setup();

	/*
	 * Only set UART up if we know which device was booted.
	 */
	tegra_uart_init();

/*
	u8 *iram = 0x40000000;
	u32 iram_size = 0x40000;
	char tmp[3];

	for (i = 0; i < iram_size; i++) {
		if (i % 16 == 0) {
			tegra_uart_print("\n");
		} else if (i % 8 == 0) {
			tegra_uart_print(" ");
		}
		tmp[0] = "0123456789ABCDEF"[*(iram+i) & 0x0f];
		tmp[1] = "0123456789ABCDEF"[(*(iram+i) >> 4) & 0x0f];
		tmp[2] = '\0';
		tegra_uart_print(tmp);
	}
*/
	/*
	 * Get a handle to the loaded image protocol.  This is used to get
	 * information about the running image, such as size and the command
	 * line.
	 */
	status = efi_system_table->boottime->handle_protocol(handle,
					&loaded_image_proto, (void *)&image);
	if (status != EFI_SUCCESS) {
		efi_err("Failed to get loaded image protocol\n");
		goto fail;
	}

	/*
	 * Load cmdline parameters like dtb=, initrd=, root= from file.
	 * The file is called "cmdline.txt" and should be located next to the kernel.
	 * This will override the cmdline passed by bootloader.
	 */
	status = efi_read_cmdline_from_file(image);
	if (status != EFI_SUCCESS) {
		efi_err("Unable to read cmdline parameters from file\n");
		goto fail;
	}

	/*
	 * Get the command line from EFI, using the LOADED_IMAGE
	 * protocol. We are going to copy the command line into the
	 * device tree, so this can be allocated anywhere.
	 */
	cmdline_ptr = efi_convert_cmdline(image, &cmdline_size);
	if (!cmdline_ptr) {
		efi_err("getting command line via LOADED_IMAGE_PROTOCOL\n");
		status = EFI_OUT_OF_RESOURCES;
		goto fail;
	}

	if (IS_ENABLED(CONFIG_CMDLINE_EXTEND) ||
	    IS_ENABLED(CONFIG_CMDLINE_FORCE) ||
	    cmdline_size == 0) {
		status = efi_parse_options(CONFIG_CMDLINE);
		if (status != EFI_SUCCESS) {
			efi_err("Failed to parse options\n");
			goto fail_free_cmdline;
		}
	}

	if (!IS_ENABLED(CONFIG_CMDLINE_FORCE) && cmdline_size > 0) {
		status = efi_parse_options(cmdline_ptr);
		if (status != EFI_SUCCESS) {
			efi_err("Failed to parse options\n");
			goto fail_free_cmdline;
		}
	}

	efi_info("Booting Linux Kernel...\n");

	si = setup_graphics();

	status = handle_kernel_image(&image_addr, &image_size,
				     &reserve_addr,
				     &reserve_size,
				     image);
	if (status != EFI_SUCCESS) {
		efi_err("Failed to relocate kernel\n");
		goto fail_free_screeninfo;
	}

	efi_retrieve_tpm2_eventlog();

	/* Ask the firmware to clear memory on unclean shutdown */
	efi_enable_reset_attack_mitigation();

	secure_boot = efi_get_secureboot();

	/*
	 * Unauthenticated device tree data is a security hazard, so ignore
	 * 'dtb=' unless UEFI Secure Boot is disabled.  We assume that secure
	 * boot is enabled if we can't determine its state.
	 *
	 * Most Windows RT devices can't disable secureboot so they need to skip
	 * this check.
	 */
	if (!IS_ENABLED(CONFIG_EFI_ARMSTUB_DTB_LOADER) ||
	    (secure_boot != efi_secureboot_mode_disabled &&
	    !IS_ENABLED(CONFIG_WINDOWS_RT_SECUREBOOT_SKIP))) {
		if (strstr(cmdline_ptr, "dtb="))
			efi_err("Ignoring DTB from command line.\n");
	} else {
		status = efi_load_dtb(image, &fdt_addr, &fdt_size);

		if (status != EFI_SUCCESS) {
			efi_err("Failed to load device tree!\n");
			goto fail_free_image;
		}
	}

	if (fdt_addr) {
		efi_info("Using DTB from command line\n");
	} else {
		/* Look for a device tree configuration table entry. */
		fdt_addr = (uintptr_t)get_fdt(&fdt_size);
		if (fdt_addr)
			efi_info("Using DTB from configuration table\n");
	}

	if (!fdt_addr)
		efi_info("Generating empty DTB\n");

	efi_load_initrd(image, &initrd_addr, &initrd_size, ULONG_MAX,
			efi_get_max_initrd_addr(image_addr));

	/*
	 * If the NX PE data feature is enabled in the properties table, we
	 * should take care not to create a virtual mapping that changes the
	 * relative placement of runtime services code and data regions, as
	 * they may belong to the same PE/COFF executable image in memory.
	 * The easiest way to achieve that is to simply use a 1:1 mapping.
	 */
	prop_tbl = get_efi_config_table(EFI_PROPERTIES_TABLE_GUID);
	flat_va_mapping = prop_tbl &&
			  (prop_tbl->memory_protection_attribute &
			   EFI_PROPERTIES_RUNTIME_MEMORY_PROTECTION_NON_EXECUTABLE_PE_DATA);

	/* force efi_novamap if SetVirtualAddressMap() is unsupported */
	efi_novamap |= !(get_supported_rt_services() &
			 EFI_RT_SUPPORTED_SET_VIRTUAL_ADDRESS_MAP);

	/* hibernation expects the runtime regions to stay in the same place */
	if (!IS_ENABLED(CONFIG_HIBERNATION) && !efi_nokaslr && !flat_va_mapping) {
		/*
		 * Randomize the base of the UEFI runtime services region.
		 * Preserve the 2 MB alignment of the region by taking a
		 * shift of 21 bit positions into account when scaling
		 * the headroom value using a 32-bit random value.
		 */
		static const u64 headroom = EFI_RT_VIRTUAL_LIMIT -
					    EFI_RT_VIRTUAL_BASE -
					    EFI_RT_VIRTUAL_SIZE;
		u32 rnd;

		status = efi_get_random_bytes(sizeof(rnd), (u8 *)&rnd);
		if (status == EFI_SUCCESS) {
			virtmap_base = EFI_RT_VIRTUAL_BASE +
				       (((headroom >> 21) * rnd) >> (32 - 21));
		}
	}

	install_memreserve_table();
/*
	for (i = 0; i < 0x1000; i++) {
		if (i % 16 == 0) {
			tegra_uart_print("\n");
			tegra_uart_print("%02x: ", i);
		}

		tegra_uart_print("%02x ", *(base+i));
	}
*/
/**
 * @brief lock Vbus GPIO
 * locks the VBus GPIO so ExitBootService call can't turn it off.
 */
/*
	uint16_t *gpio = (uint16_t*) 0x6000d208;
	uint16_t cur_val = *gpio;
	cur_val = cur_val | 0x200;
	*gpio = cur_val;
*/
	status = allocate_new_fdt_and_exit_boot(handle, &fdt_addr,
						initrd_addr, initrd_size,
						cmdline_ptr, fdt_addr, fdt_size);
/*
	for (i = 0; i < 0x1000; i++) {
		if (i % 16 == 0) {
			tegra_uart_print("\n");
			tegra_uart_print("%02x: ", i);
		}

		tegra_uart_print("%02x ", *(base+i));
	}
*/
	if (status != EFI_SUCCESS)
		goto fail_free_initrd;

	if (IS_ENABLED(CONFIG_ARM))
		efi_handle_post_ebs_state();

	efi_enter_kernel(image_addr, fdt_addr, fdt_totalsize((void *)fdt_addr));
	/* not reached */

fail_free_initrd:
	efi_err("Failed to update FDT and exit boot services\n");

	efi_free(initrd_size, initrd_addr);
	efi_free(fdt_size, fdt_addr);

fail_free_image:
	efi_free(image_size, image_addr);
	efi_free(reserve_size, reserve_addr);
fail_free_screeninfo:
	free_screen_info(si);
fail_free_cmdline:
	efi_bs_call(free_pool, cmdline_ptr);
fail:
	return status;
}

/*
 * efi_get_virtmap() - create a virtual mapping for the EFI memory map
 *
 * This function populates the virt_addr fields of all memory region descriptors
 * in @memory_map whose EFI_MEMORY_RUNTIME attribute is set. Those descriptors
 * are also copied to @runtime_map, and their total count is returned in @count.
 */
void efi_get_virtmap(efi_memory_desc_t *memory_map, unsigned long map_size,
		     unsigned long desc_size, efi_memory_desc_t *runtime_map,
		     int *count)
{
	u64 efi_virt_base = virtmap_base;
	efi_memory_desc_t *in, *out = runtime_map;
	int l;

	for (l = 0; l < map_size; l += desc_size) {
		u64 paddr, size;

		in = (void *)memory_map + l;
		if (!(in->attribute & EFI_MEMORY_RUNTIME))
			continue;

		paddr = in->phys_addr;
		size = in->num_pages * EFI_PAGE_SIZE;

		in->virt_addr = in->phys_addr;
		if (efi_novamap) {
			continue;
		}

		/*
		 * Make the mapping compatible with 64k pages: this allows
		 * a 4k page size kernel to kexec a 64k page size kernel and
		 * vice versa.
		 */
		if (!flat_va_mapping) {

			paddr = round_down(in->phys_addr, SZ_64K);
			size += in->phys_addr - paddr;

			/*
			 * Avoid wasting memory on PTEs by choosing a virtual
			 * base that is compatible with section mappings if this
			 * region has the appropriate size and physical
			 * alignment. (Sections are 2 MB on 4k granule kernels)
			 */
			if (IS_ALIGNED(in->phys_addr, SZ_2M) && size >= SZ_2M)
				efi_virt_base = round_up(efi_virt_base, SZ_2M);
			else
				efi_virt_base = round_up(efi_virt_base, SZ_64K);

			in->virt_addr += efi_virt_base - paddr;
			efi_virt_base += size;
		}

		memcpy(out, in, desc_size);
		out = (void *)out + desc_size;
		++*count;
	}
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////  I2C  ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define vu32 volatile u32

static void _i2c_wait(vu32 *base)
{
	u32 i;
	base[0x23] = 0x25;
	for (i = 0; i < 20; i++)
	{
		if (!(base[0x23] & 1))
			break;
	}
}

/**
 * @brief i2c send package
 * Sends a package to the I2C Bus
 * @param idx i2c controller
 * @param x slave address
 * @param buf data to be transmitted
 * @param size data size
 * @return int error code
 */
static int _i2c_send_pkt(u32 idx, u32 x, u8 *buf, u32 size)
{
	vu32 *base = (vu32 *) 0x7000d000; //i2c_addrs[idx];
	u32 tmp = 0;
	if (size > 4)
		return 0;

	memcpy(&tmp, buf, size);

	base[1] = x << 1; //Set x (send mode). x = address + 0=write
	base[3] = tmp; //Set value. data register
	base[0] = (2 * size - 2) | 0x2800; //Set size and send mode.
	_i2c_wait(base); //Kick transaction.

	base[0] = (base[0] & 0xFFFFFDFF) | 0x200;
	while (base[7] & 0x100)
		;

	if (base[7] << 28)
		return 0;

	return 1;
}

static int _i2c_recv_pkt(u32 idx, u8 *buf, u32 size, u32 x)
{
	vu32 *base = (vu32 *) 0x7000d000; //i2c_addrs[idx];
	u32 tmp;

	if (size > 4)
		return 0;

	base[1] = (x << 1) | 1; //Set x (recv mode).
	base[0] = (2 * size - 2) | 0x2840; //Set size and recv mode.
	_i2c_wait(base); //Kick transaction.

	base[0] = (base[0] & 0xFFFFFDFF) | 0x200;
	while (base[7] & 0x100)
		;

	if (base[7] << 28)
		return 0;

	tmp = base[3]; //Get value.
	memcpy(buf, &tmp, size);

	return 1;
}

int i2c_recv_buf_small(u8 *buf, u32 size, u32 idx, u32 x, u32 y)
{
	int res = _i2c_send_pkt(idx, x, (u8 *)&y, 1);
	if (res)
		res = _i2c_recv_pkt(idx, buf, size, x);
	return res;
}