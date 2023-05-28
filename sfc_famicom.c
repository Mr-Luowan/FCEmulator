#include "sfc_famicom.h"
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

// ����Ĭ��ROM
static sfc_ecode sfc_load_default_rom(void*, sfc_rom_info_t*);
// �ͷ�Ĭ��ROM
static sfc_ecode sfc_free_default_rom(void*, sfc_rom_info_t*);

//����һ������ָ��
typedef void(*sfc_funcptr_t)();

sfc_ecode sfc_famicom_init(sfc_famicom_t* famicom, void* argument, const sfc_interface_t* interfaces)
{
	famicom->argument = argument;
	//����Ĭ�Ͻӿ�
	famicom->interfaces.load_rom = sfc_load_default_rom;
	famicom->interfaces.free_rom = sfc_free_default_rom;
	//�������
	memset(&famicom->rom_info, 0, sizeof(famicom->rom_info));
	printf("��ʼ����ʼ\n");
	//�ṩ�˽ӿ�
	if (interfaces)
	{
		const size_t count = sizeof(*interfaces) / sizeof(interfaces->load_rom);
		//������Ч�ĺ���ָ��
		//UB:C��׼��һ����֤sizeof(void*)��ͬsizeof(fp) (�Ƿ���ϵ) ��������������һ������ָ��sfc_funcptr_t
		sfc_funcptr_t* const func_src = (sfc_funcptr_t*)interfaces;
		sfc_funcptr_t* const func_des = (sfc_funcptr_t*)&famicom->interfaces;
		for (size_t i = 0; i < count; i++)
		{
			if (func_src[i])
			{
				func_des[i] = func_src[i];
			}
		}
	}
	printf("��ʼ���ɹ�\n");
	// һ��ʼ�������ROM
	return famicom->interfaces.load_rom(argument, &famicom->rom_info);
	return SFC_OK;
}

void sfc_famicom_uninit(sfc_famicom_t* famicom)
{
	famicom->interfaces.free_rom(famicom->argument, &famicom->rom_info);
}


/// <summary>
/// ����Ĭ��ROM
/// </summary>
/// <param name="arg"></param>
/// <param name="info"></param>
/// <returns></returns>
sfc_ecode sfc_load_default_rom(void* arg, sfc_rom_info_t* info)
{
	FILE* const file = fopen("nestest.nes", "rb");
	printf("��ȡROM�ļ�\n");
	if (!file)
	{
		printf("��ȡROM�ļ�ʧ��\n");
		return SFC_ERROR_FILE_NOT_FOUND;
	}
	sfc_ecode code = SFC_ERROR_ILLEGAL_FILE;
	sfc_nes_header_t nes_header;
	if (fread(&nes_header, sizeof(nes_header), 1, file))
	{
		//��ͷ�ĸ��ֽ�
		union 
		{
			uint32_t u32;
			uint8_t id[4];
		} this_union;
		this_union.id[0] = 'N';
		this_union.id[1] = 'E';
		this_union.id[2] = 'S';
		this_union.id[3] = '\x1A';
		//�Ƚ����ĸ��ֽ�
		if (this_union.u32 == nes_header.id)
		{
			const size_t size1 = 16 * 1024 * nes_header.count_progrom16kb;
			const size_t size2 = 8 * 1024 * nes_header.count_chrrom_8kb;
			uint8_t* const ptr = (uint8_t*)malloc(size1 + size2);
			//�����ڴ�ɹ�
			if (ptr)
			{
				code = SFC_OK;
				if (nes_header.control1 & SFC_NES_TRAINER)
				{
					// ����Trainer����
					fseek(file, 512, SEEK_CUR);
				}
				fread(ptr, size1 + size2, 1, file);
				//��дinfo���ݱ��
				info->data_progrom = ptr;
				info->data_chrrom = ptr + size1;
				info->count_program16kb = nes_header.count_progrom16kb;
				info->count_chrrom_8kb = nes_header.count_chrrom_8kb;
				info->mapper_number =
					(nes_header.control1 >> 4) | (nes_header.control2 & 0xF0);
				info->vmirroring = (nes_header.control1 & SFC_NES_VMIRROR) > 0;
				info->save_ram = (nes_header.control1 & SFC_NES_SAVERAM) > 0;
				info->four_screen = (nes_header.control1 & SFC_NES_4SCREEN) > 0;
			}
			else
			{
				//todo �ڴ治��
				code = SFC_ERROR_OUT_OF_MEMORY;
			}
		}
	}
	fclose(file);
	return code;
}

/// <summary>
///  �ͷ�Ĭ�ϲ���ROM
/// </summary>
/// <param name="arg"></param>
/// <param name="info"></param>
/// <returns></returns>
sfc_ecode sfc_free_default_rom(void* arg, sfc_rom_info_t* info)
{
	// �ͷŶ�̬���������
	free(info->data_progrom);
	info->data_progrom = NULL;
	return SFC_OK;
}