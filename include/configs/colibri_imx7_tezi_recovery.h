/*
 * Copyright 2016-2017 Toradex AG
 *
 * Configuration settings for the Colibri iMX7 module.
 *
 * based on mx7dsabresd.h:
 * Copyright (C) 2015 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __COLIBRI_IMX7_CONFIG_H
#define __COLIBRI_IMX7_CONFIG_H

#include "mx7_common.h"

/*#define CONFIG_DBG_MONITOR*/
#define PHYS_SDRAM_SIZE			SZ_512M

#define CONFIG_DISPLAY_BOARDINFO_LATE	/* Calls show_board_info() */

#define CONFIG_ENV_VARS_UBOOT_CONFIG
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(32 * SZ_1M)

/* MMC Config*/
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_USDHC_NUM	1

#undef CONFIG_BOOTM_PLAN9
#undef CONFIG_BOOTM_RTEMS

/* I2C configs */
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_IPADDR			192.168.10.2
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_SERVERIP			192.168.10.1

/* can't determine the u-boot update variant in the recovery u-boot */
#define UBOOT_UPDATE ""

#define MEM_LAYOUT_ENV_SETTINGS \
	"bootm_size=0x10000000\0" \
	"fdt_addr_r=0x82000000\0" \
	"initrd_high=0xffffffff\0" \
	"kernel_addr_r=0x81000000\0" \
	"pxefile_addr_r=0x87100000\0" \
	"ramdisk_addr_r=0x82100000\0" \
	"scriptaddr=0x87000000\0"

#define CONFIG_BOOTCOMMAND "run ubiboot; " \
	"setenv fdtfile ${soc}-colibri-${fdt_board}.dtb && run distro_bootcmd"

#undef BOOTENV_RUN_NET_USB_START

/* No module specific environment in the recovery u-boot */
#define MODULE_EXTRA_ENV_SETTINGS ""

#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#include <config_distro_bootcmd.h>

#undef CONFIG_ISO_PARTITION

#if defined(CONFIG_TDX_EASY_INSTALLER)
#  define BOOT_SCRIPT	"boot-tezi.scr"
#else
#  define BOOT_SCRIPT   "boot.scr"
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	BOOTENV \
	MEM_LAYOUT_ENV_SETTINGS \
	MODULE_EXTRA_ENV_SETTINGS \
	UBOOT_UPDATE \
	"boot_scripts=" BOOT_SCRIPT "\0" \
	"boot_script_dhcp=" BOOT_SCRIPT "\0" \
	"boot_file=zImage\0" \
	"bootm_boot_mode=sec\0" \
	"bootubipart=ubi\0" \
	"console=ttymxc0\0" \
	"defargs=\0" \
	"fdt_board=eval-v3\0" \
	"fdt_fixup=;\0" \
	"m4boot=;\0" \
	"ip_dyn=yes\0" \
	"kernel_file=zImage\0" \
	"setethupdate=if env exists ethaddr; then; else setenv ethaddr " \
		"00:14:2d:00:00:00; fi; tftpboot ${loadaddr} " \
		"${board}/flash_eth.img && source ${loadaddr}\0" \
	"setsdupdate=mmc rescan && setenv interface mmc && " \
		"fatload ${interface} 0:1 ${loadaddr} " \
		"${board}/flash_blk.img && source ${loadaddr}\0" \
	"setup=setenv setupargs " \
		"console=tty1 console=${console}" \
		",${baudrate}n8 ${memargs} consoleblank=0\0" \
	"setupdate=run setsdupdate || run setusbupdate || run setethupdate\0" \
	"setusbupdate=usb start && setenv interface usb && " \
		"fatload ${interface} 0:1 ${loadaddr} " \
		"${board}/flash_blk.img && source ${loadaddr}\0" \
	"splashpos=m,m\0" \
	"splashimage=" __stringify(CONFIG_LOADADDR) "\0" \
	"videomode=video=ctfb:x:640,y:480,depth:18,pclk:39722,le:48,ri:16,up:33,lo:10,hs:96,vs:2,sync:0,vmode:0\0" \
	"updlevel=2\0" \
	"vidargs=video=mxsfb:640x480M-16@60"

/* Miscellaneous configurable options */
#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* FLASH and environment organization */
/* Dynamic MTD partition support */
#define CONFIG_CMD_MTDPARTS	/* Enable 'mtdparts' command line support */
#define MTDPARTS_DEFAULT	"mtdparts=gpmi-nand:"		\
				"512k(mx7-bcb),"		\
				"1536k(u-boot1)ro,"		\
				"1536k(u-boot2)ro,"		\
				"512k(u-boot-env),"		\
				"-(ubi)"

/* USB Configs */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET

#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2

#define CONFIG_IMX_THERMAL

#define CONFIG_USBD_HS

/* USB Device Firmware Update support */
#define CONFIG_SYS_DFU_DATA_BUF_SIZE	SZ_16M
#define DFU_DEFAULT_POLL_TIMEOUT	300

#if defined(CONFIG_VIDEO) || defined(CONFIG_DM_VIDEO)
#define CONFIG_VIDEO_MXS
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_CMD_BMP
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

#endif
