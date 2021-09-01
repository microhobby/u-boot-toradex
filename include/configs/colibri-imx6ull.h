/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018-2021 Toradex AG
 *
 * Configuration settings for the Colibri iMX6ULL module.
 *
 * based on colibri_imx7.h
 */

#ifndef __COLIBRI_IMX6ULL_CONFIG_H
#define __COLIBRI_IMX6ULL_CONFIG_H

#include "mx6_common.h"
#define CONFIG_IOMUX_LPSR

#define PHYS_SDRAM_SIZE			SZ_1G

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(32 * SZ_1M)

/* Network */
#define CONFIG_TFTP_TSIZE

/* ENET1 */
#define IMX_FEC_BASE			ENET2_BASE_ADDR

/* MMC Config */
#define CONFIG_SYS_FSL_ESDHC_ADDR	0
#define CONFIG_SYS_FSL_USDHC_NUM	1

/* I2C configs */
#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_IPADDR			192.168.10.2
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_SERVERIP			192.168.10.1
#define CONFIG_ROOTPATH			"/srv/nfs"

#if defined(CONFIG_TARGET_COLIBRI_IMX6ULL_EMMC)
#define UBOOT_UPDATE \
	"uboot_hwpart=1\0" \
	"uboot_blk=2\0" \
	"set_blkcnt=setexpr blkcnt ${filesize} + 0x1ff && " \
		"setexpr blkcnt ${blkcnt} / 0x200\0" \
	"update_uboot=run set_blkcnt && mmc dev 0 ${uboot_hwpart} && " \
		"mmc write ${loadaddr} ${uboot_blk} ${blkcnt}\0"
#elif defined(CONFIG_TARGET_COLIBRI_IMX6ULL_NAND)
#define UBOOT_UPDATE \
	"update_uboot=nand erase.part u-boot1 && " \
		"nand write ${loadaddr} u-boot1 ${filesize} && " \
		"nand erase.part u-boot2 && " \
		"nand write ${loadaddr} u-boot2 ${filesize}\0"
#endif

#define MEM_LAYOUT_ENV_SETTINGS \
	"bootm_size=0x10000000\0" \
	"fdt_addr_r=0x82100000\0" \
	"kernel_addr_r=0x81000000\0" \
	"pxefile_addr_r=0x87100000\0" \
	"ramdisk_addr_r=0x82200000\0" \
	"scriptaddr=0x87000000\0"

#ifdef CONFIG_TDX_EASY_INSTALLER
#define UBI_BOOTCMD \
        "ubiargs=ubi.mtd=ubi root=ubi0:rootfs rw rootfstype=ubifs " \
                "ubi.fm_autoconvert=1\0" \
        "ubiboot=run setup; " \
                "setenv bootargs console=ttymxc0,115200 quiet " \
                "rootfstype=squashfs root=/dev/ram autoinstall " \
                "${teziargs}; echo Booting Toradex Easy Installer...; " \
                "ubi part ubi && " \
                "ubi read ${ramdisk_addr_r} rootfs && " \
                "bootm ${ramdisk_addr_r}\0"
#else /* CONFIG_TDX_EASY_INSTALLER */
#define UBI_BOOTCMD \
	"ubiargs=ubi.mtd=ubi root=ubi0:rootfs rw rootfstype=ubifs " \
		"ubi.fm_autoconvert=1\0" \
	"ubiboot=run setup; " \
		"setenv bootargs ${defargs} ${ubiargs} " \
		"${setupargs} ${vidargs} ${tdxargs}; echo Booting from NAND...; " \
		"ubi part ubi &&" \
		"ubi read ${kernel_addr_r} kernel && " \
		"ubi read ${fdt_addr_r} dtb && " \
		"run fdt_fixup && bootz ${kernel_addr_r} - ${fdt_addr_r}\0"
#endif /* CONFIG_TDX_EASY_INSTALLER */

#if defined(CONFIG_TARGET_COLIBRI_IMX6ULL_NAND)
#define DFU_ALT_NAND_INFO "imx6ull-bcb part 0,1;u-boot1 part 0,2;u-boot2 part 0,3;u-boot-env part 0,4;ubi partubi 0,5"
#define MODULE_EXTRA_ENV_SETTINGS \
	"dfu_alt_info=" DFU_ALT_NAND_INFO "\0" \
	"mtdparts=" CONFIG_MTDPARTS_DEFAULT "\0" \
	UBI_BOOTCMD
#else
#define	MODULE_EXTRA_ENV_SETTINGS ""
#endif

#if defined(CONFIG_TARGET_COLIBRI_IMX6ULL_NAND)
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#elif defined(CONFIG_TARGET_COLIBRI_IMX6ULL_EMMC)
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
	func(DHCP, dhcp, na)
#endif
#include <config_distro_bootcmd.h>

#if defined(CONFIG_TDX_EASY_INSTALLER)
#  define BOOT_SCRIPT	"boot-tezi.scr"
#else
#  define BOOT_SCRIPT   "boot.scr"
#endif

#define VIDARGS	""

#define CONFIG_EXTRA_ENV_SETTINGS \
	BOOTENV \
	MEM_LAYOUT_ENV_SETTINGS \
	MODULE_EXTRA_ENV_SETTINGS \
	UBOOT_UPDATE \
	"boot_scripts=" BOOT_SCRIPT "\0" \
	"boot_script_dhcp=" BOOT_SCRIPT "\0" \
	"bootubipart=ubi\0" \
	"console=ttymxc0\0" \
	"defargs=user_debug=30\0" \
	"fdt_board=eval-v3\0" \
	"fdt_fixup=;\0" \
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
	"vidargs=" VIDARGS

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR

/* Physical Memory Map */
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* environment organization */

#if defined(CONFIG_ENV_IS_IN_MMC)
/* Environment in eMMC, before config block at the end of 1st "boot sector" */
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_SYS_MMC_ENV_PART		1
#elif defined(CONFIG_ENV_IS_IN_NAND)
#define CONFIG_ENV_RANGE	(4 * CONFIG_ENV_SIZE)
#endif

#ifdef CONFIG_TARGET_COLIBRI_IMX6ULL_NAND
/* NAND stuff */
#define CONFIG_SYS_MAX_NAND_DEVICE	1
/* used to initialize CONFIG_SYS_NAND_BASE_LIST which is unused */
#define CONFIG_SYS_NAND_BASE		-1
#define CONFIG_SYS_NAND_ONFI_DETECTION
#endif

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
#define MXS_LCDIF_BASE MX6UL_LCDIF1_BASE_ADDR
#define CONFIG_VIDEO_LOGO
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_VIDEO_BMP_LOGO
#endif

#endif /* __COLIBRI_IMX6ULL_CONFIG_H */
