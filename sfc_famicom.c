#include "sfc_famicom.h"
#include <string.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

// 加载默认ROM
static sfc_ecode sfc_load_default_rom(void*, sfc_rom_info_t*);
// 释放默认ROM
static sfc_ecode sfc_free_default_rom(void*, sfc_rom_info_t*);

//声明一个函数指针
typedef void(*sfc_funcptr_t)();

sfc_ecode sfc_famicom_init(sfc_famicom_t* famicom, void* argument, const sfc_interface_t* interfaces)
{
	famicom->argument = argument;
	//载入默认接口
	famicom->interfaces.load_rom = sfc_load_default_rom;
	famicom->interfaces.free_rom = sfc_free_default_rom;
	//清空数据
	memset(&famicom->rom_info, 0, sizeof(famicom->rom_info));
	printf("初始化开始\n");
	//提供了接口
	if (interfaces)
	{
		const size_t count = sizeof(*interfaces) / sizeof(interfaces->load_rom);
		//复制有效的函数指针
		//UB:C标准并一定保证sizeof(void*)等同sizeof(fp) (非冯体系) 所以这里声明了一个函数指针sfc_funcptr_t
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
	printf("初始化成功\n");
	// 一开始载入测试ROM
	return famicom->interfaces.load_rom(argument, &famicom->rom_info);
	return SFC_OK;
}

void sfc_famicom_uninit(sfc_famicom_t* famicom)
{
	famicom->interfaces.free_rom(famicom->argument, &famicom->rom_info);
}


/// <summary>
/// 加载默认ROM
/// </summary>
/// <param name="arg"></param>
/// <param name="info"></param>
/// <returns></returns>
sfc_ecode sfc_load_default_rom(void* arg, sfc_rom_info_t* info)
{
	FILE* const file = fopen("nestest.nes", "rb");
	printf("读取ROM文件\n");
	if (!file)
	{
		printf("读取ROM文件失败\n");
		return SFC_ERROR_FILE_NOT_FOUND;
	}
	sfc_ecode code = SFC_ERROR_ILLEGAL_FILE;
	sfc_nes_header_t nes_header;
	if (fread(&nes_header, sizeof(nes_header), 1, file))
	{
		//开头四个字节
		union 
		{
			uint32_t u32;
			uint8_t id[4];
		} this_union;
		this_union.id[0] = 'N';
		this_union.id[1] = 'E';
		this_union.id[2] = 'S';
		this_union.id[3] = '\x1A';
		//比较这四个字节
		if (this_union.u32 == nes_header.id)
		{
			const size_t size1 = 16 * 1024 * nes_header.count_progrom16kb;
			const size_t size2 = 8 * 1024 * nes_header.count_chrrom_8kb;
			uint8_t* const ptr = (uint8_t*)malloc(size1 + size2);
			//申请内存成功
			if (ptr)
			{
				code = SFC_OK;
				if (nes_header.control1 & SFC_NES_TRAINER)
				{
					// 跳过Trainer数据
					fseek(file, 512, SEEK_CUR);
				}
				fread(ptr, size1 + size2, 1, file);
				//填写info数据表格
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
				//todo 内存不足
				code = SFC_ERROR_OUT_OF_MEMORY;
			}
		}
	}
	fclose(file);
	return code;
}

/// <summary>
///  释放默认测试ROM
/// </summary>
/// <param name="arg"></param>
/// <param name="info"></param>
/// <returns></returns>
sfc_ecode sfc_free_default_rom(void* arg, sfc_rom_info_t* info)
{
	// 释放动态申请的数据
	free(info->data_progrom);
	info->data_progrom = NULL;
	return SFC_OK;
}