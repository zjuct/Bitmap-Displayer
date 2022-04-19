#pragma once
#ifndef _UTIL_H_
#define _UTIL_H_
#include<stdio.h>
#include<stdlib.h>

/*
by ��ͩ
last_modified:2022-04-18 23:47
��ͷ�ļ�������һϵ�й���
*/

//RGB/RGBA��ɫ�ṹ�嶨��
typedef struct {
    unsigned char B;
    unsigned char G;
    unsigned char R;
}RGB;

typedef struct {
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
}RGBA;

//���ش�����
typedef struct {
    unsigned char length;
    unsigned short int value;
}bitstring;

//�����Ʊ���
static bitstring* code_u;
static bitstring* code_d; //�����Լ�����

//��ɫ����������
RGB* rgb_buffer;
RGBA* rgba_buffer;
FILE* fp_jpg;
signed short int img_width, img_height;

static unsigned char bytecode = 0;
static signed char bytepos = 8;

#define PI 3.1415926

//д��һ���ֽ�
#define writebyte(b) fputc((b), fp_jpg)
//д�������ֽڣ���λ��ǰ����λ�ں�
#define writeword(w) writebyte((w)/256);writebyte((w)%256);

//YCbCr��ɫ�ռ����
static signed char YDU[64];
static signed char CbDU[64];
static signed char CrDU[64];

//����Zig-Zag������õ���һά����
static signed short int ZigZagBuffer[64];

//Zig-Zag��������
static unsigned char ZigZagCode[64] =
{ 0, 1, 5, 6,14,15,27,28,
  2, 4, 7,13,16,26,29,42,
  3, 8,12,17,25,30,41,43,
  9,11,18,24,31,40,44,53,
 10,19,23,32,39,45,52,54,
 20,22,33,38,46,51,55,60,
 21,34,37,47,50,56,59,61,
 35,36,48,49,57,58,62,63 };

//��λȡֵ��
static unsigned short int mark[16] = { 1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768 };


#endif