#pragma once

#include "sfc_rom.h"
#include "sfc_code.h"

/*
* FC扩展接口
*/
typedef struct {
	//ROM 加载器读取信息
	sfc_ecode(*load_rom)(void*, sfc_rom_info_t*);
	//ROM 加载器卸载
	sfc_ecode(*free_rom)(void*, sfc_rom_info_t*);
} sfc_interface_t;

/*
* 模拟器主体
*/
typedef struct {
	//参数
	void* argument;
	//接口
	sfc_interface_t interfaces;
	//rom信息
	sfc_rom_info_t rom_info;
} sfc_famicom_t;

//初始化
sfc_ecode sfc_famicom_init(
	sfc_famicom_t* famicom,
	void* argument,
	const sfc_interface_t* interfaces
);

//反初始化
void sfc_famicom_uninit(sfc_famicom_t* famicom);