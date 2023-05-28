#pragma once

#include "sfc_rom.h"
#include "sfc_code.h"

/*
* FC��չ�ӿ�
*/
typedef struct {
	//ROM ��������ȡ��Ϣ
	sfc_ecode(*load_rom)(void*, sfc_rom_info_t*);
	//ROM ������ж��
	sfc_ecode(*free_rom)(void*, sfc_rom_info_t*);
} sfc_interface_t;

/*
* ģ��������
*/
typedef struct {
	//����
	void* argument;
	//�ӿ�
	sfc_interface_t interfaces;
	//rom��Ϣ
	sfc_rom_info_t rom_info;
} sfc_famicom_t;

//��ʼ��
sfc_ecode sfc_famicom_init(
	sfc_famicom_t* famicom,
	void* argument,
	const sfc_interface_t* interfaces
);

//����ʼ��
void sfc_famicom_uninit(sfc_famicom_t* famicom);