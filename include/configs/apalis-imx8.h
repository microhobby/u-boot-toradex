/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright 2019-2022 Toradex
 */

#ifndef __APALIS_IMX8_H
#define __APALIS_IMX8_H

#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>

/* Networking */
#define CONFIG_IPADDR			192.168.10.2
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_SERVERIP			192.168.10.1

#define MEM_LAYOUT_ENV_SETTINGS \
	"fdt_addr_r=0x9d400000\0" \
	"kernel_addr_r=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"kernel_comp_addr_r=0xf0000000\0" \
	"kernel_comp_size=0x08000000\0" \
	"ramdisk_addr_r=0x9d500000\0" \
	"scriptaddr=0x9d480000\0"

/* Boot M4 */
#define M4_BOOT_ENV \
	"m4_0_image=m4_0.bin\0" \
	"m4_1_image=m4_1.bin\0" \
	"loadm4image_0=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${m4_0_image}\0" \
	"loadm4image_1=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${m4_1_image}\0" \
	"m4boot_0=run loadm4image_0; dcache flush; bootaux ${loadaddr} 0\0" \
	"m4boot_1=run loadm4image_1; dcache flush; bootaux ${loadaddr} 1\0" \

/* Enable Distro Boot */
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 2) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#include <config_distro_bootcmd.h>

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS \
	BOOTENV \
	M4_BOOT_ENV \
	MEM_LAYOUT_ENV_SETTINGS \
	"boot_script_dhcp=boot.scr\0" \
	"console=ttyLP1\0" \
	"fdt_board=eval\0" \
	"initrd_addr=0x83800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"setup=setenv setupargs console=tty1 console=${console},${baudrate} " \
		"consoleblank=0 earlycon\0" \
	"update_uboot=askenv confirm Did you load flash.bin resp. u-boot-dtb.imx (y/N)?; " \
		"if test \"$confirm\" = \"y\"; then " \
		"setexpr blkcnt ${filesize} + 0x1ff && setexpr blkcnt " \
		"${blkcnt} / 0x200; mmc dev 0 1; mmc write ${loadaddr} 0x0 " \
		"${blkcnt}; fi\0"

#define CONFIG_SYS_INIT_SP_ADDR		0x80200000

#define CONFIG_SYS_BOOTM_LEN		SZ_64M /* Increase max gunzip size */

#define CONFIG_SYS_SDRAM_BASE		0x80000000
#define PHYS_SDRAM_1			0x80000000
#define PHYS_SDRAM_2			0x880000000
#define PHYS_SDRAM_1_SIZE		SZ_2G		/* 2 GB */
#define PHYS_SDRAM_2_SIZE		SZ_2G		/* 2 GB */

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		SZ_2K
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

/* Generic Timer Definitions */
#define COUNTER_FREQUENCY		8000000	/* 8MHz */

/* USB Config */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_USBD_HS
#endif

#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

/* USB OTG controller configs */
#ifdef CONFIG_USB_EHCI_HCD
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#endif

#endif /* __APALIS_IMX8_H */
