#pragma once

#include <stdint.h>

/*
 * ROM信息
 */
typedef struct {
	//PRG-ROM 程序为只读存储器 数据指针
	uint8_t* data_progrom;
	//CHR-ROM 角色只读存储器 数据指针
	uint8_t* data_chrrom;
	//16KB 程序只读存储器	数据长度
	uint32_t count_program16kb;
	//8KB	角色只读存储器 数据长度
	uint32_t count_chrrom_8kb;
	//mapper编号
	uint8_t mapper_number;
	//是否 Vertical Mirroring
	uint8_t vmirroring;
	uint8_t four_screen;
	//是否有sram（电池供电的）
	uint8_t save_ram;
	//保留以对齐
	uint8_t reserved[4];
}sfc_rom_info_t;

/*
	*  nes文件头
	*/
typedef struct {
	// NES^Z
	uint32_t id;
	// 16k 程序只读储存器 数量
	uint8_t count_progrom16kb;
	//  8k 角色只读存储器 数量
	uint8_t count_chrrom_8kb;
	// 控制信息1
	uint8_t control1;
	// 控制信息2
	uint8_t control2;
	// 保留数据
	uint8_t reserved[];
} sfc_nes_header_t;

/*
*  ROM control 字节 #1
*/
enum {
	SFC_NES_VMIRROR = 0x01,
	SFC_NES_SAVERAM = 0x02,
	SFC_NES_TRAINER = 0x04,
	SFC_NES_4SCREEN = 0x08
};

/*
*  ROM control 字节 #2
*/
enum {
	SFC_NES_VS_UNISYSTEM = 0x01,
	SFC_NES_PLAYCHOICE10 = 0x02
};