#pragma once
#ifndef _BMP_H_
#define _BMP_H_
#include"util.h"

/*
by ��ͩ
last_modified:2022-04-18 23:47
��ͷ�ļ�������BMP�ṹ�壬����װ��BMP�ļ��Ķ�ȡ��д���ӿ�
*/

#define MAX_SIZE 1024

static unsigned int expendX = 0;
static unsigned int expendY = 0;

//BMP�ļ�ͷ����
//�ļ���Ϣ�ṹ�� ����14�ֽ� 
typedef struct bitmap_file_header {
	unsigned short file_type;
	unsigned int file_size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset_bits;
}bitmap_file_header;

//λͼ��Ϣ�ṹ�� ,��40�ֽ� 
typedef struct bitmap_info_header {
	unsigned int bitmap_info_size;
	int bitmap_width;
	int bitmap_height;
	unsigned short planes;
	unsigned short image_depth;
	unsigned int compression;
	unsigned int image_size;
	int x_pels_permeter;
	int y_pels_permeter;
	unsigned int color_used;
	unsigned int color_important;
}bitmap_info_header;

//BMP�ṹ�嶨��
typedef struct _Bitmap {
	double x;
	double y;
	unsigned int expendX;
	unsigned int expendY;
	bitmap_file_header file_header;
	bitmap_info_header info_header;
	RGBA* rgbacolor;
	RGB* rgbcolor;
	int pos;
}Bitmap;

//��������
Bitmap* LoadBMP(char* path);
unsigned int to_unsigned_int(int begin, int end);
unsigned short to_unsigned_short(int begin, int end);
void GetFileHeader(Bitmap* bmp);
void GetInfoHeader(Bitmap* bmp);
void set_unsigned_short(FILE* fp, unsigned short num);
void set_unsigned_int(FILE* fp, unsigned short num);
void set_file_header(FILE* fp, Bitmap* pb);
void set_info_header(FILE* fp, Bitmap* pb);
void SaveBMP(char* path, Bitmap* bmp);
#endif