#pragma once

#include <stdint.h>

/*
 * ROM��Ϣ
 */
typedef struct {
	//PRG-ROM ����Ϊֻ���洢�� ����ָ��
	uint8_t* data_progrom;
	//CHR-ROM ��ɫֻ���洢�� ����ָ��
	uint8_t* data_chrrom;
	//16KB ����ֻ���洢��	���ݳ���
	uint32_t count_program16kb;
	//8KB	��ɫֻ���洢�� ���ݳ���
	uint32_t count_chrrom_8kb;
	//mapper���
	uint8_t mapper_number;
	//�Ƿ� Vertical Mirroring
	uint8_t vmirroring;
	uint8_t four_screen;
	//�Ƿ���sram����ع���ģ�
	uint8_t save_ram;
	//�����Զ���
	uint8_t reserved[4];
}sfc_rom_info_t;

/*
	*  nes�ļ�ͷ
	*/
typedef struct {
	// NES^Z
	uint32_t id;
	// 16k ����ֻ�������� ����
	uint8_t count_progrom16kb;
	//  8k ��ɫֻ���洢�� ����
	uint8_t count_chrrom_8kb;
	// ������Ϣ1
	uint8_t control1;
	// ������Ϣ2
	uint8_t control2;
	// ��������
	uint8_t reserved[];
} sfc_nes_header_t;

/*
*  ROM control �ֽ� #1
*/
enum {
	SFC_NES_VMIRROR = 0x01,
	SFC_NES_SAVERAM = 0x02,
	SFC_NES_TRAINER = 0x04,
	SFC_NES_4SCREEN = 0x08
};

/*
*  ROM control �ֽ� #2
*/
enum {
	SFC_NES_VS_UNISYSTEM = 0x01,
	SFC_NES_PLAYCHOICE10 = 0x02
};