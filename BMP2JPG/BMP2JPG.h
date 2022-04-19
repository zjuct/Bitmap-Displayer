#pragma once
#include"JPG.h"
#include"HuffmanCode.h"
#include"Qualization.h"
#include"util.h"
#include"BMP.h"

/*
by ��ͩ
last_modified:2022-04-18 23:47
��ͷ�ļ�ʵ���˽�BMP�ļ�ת��ΪJPG�ļ�
*/

//��������
void Bmp2Jpg(char* bmp_path, char* jpg_path);
void InitJpgHead();
void WriteJpgHead(char* path);
void Encode(Bitmap* bmp);
void InitHuffmanCode(unsigned char* codes, unsigned char* values, bitstring* HT);
void leng_and_numbercode();
void RGB2YcbCr(Bitmap* bmp, int y, int x);
void DCT(signed char* matrix, double(*res)[8]);
void Qualization(double(*matrix)[8], unsigned char* table, signed short int* res);
void ZigZagTraverse(signed short* matrix);
void HuffmanEncode(signed short int* DC, bitstring* DCTable, bitstring* ACTable);
void writefile(bitstring a);