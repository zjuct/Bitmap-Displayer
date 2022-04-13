#pragma warning(disable:4996)
/*
���ļ�����bmp�ļ��Ķ����д��

*/

#include<Windows.h>
#include<stdio.h>
#include<stdlib.h>


#define MAX_SIZE 4096

typedef struct pair {
	unsigned char ch1;
	unsigned char ch2;
}pair;

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

typedef struct bitmap_palette {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char reserved;
}bitmap_palette;

typedef struct Bitmap {
	double x;
	double y;
	bitmap_file_header file_header;
	bitmap_info_header info_header;
	bitmap_palette color[MAX_SIZE * MAX_SIZE];
	int pos;
}Bitmap;

//������charƴ��Ϊunsigned short
unsigned short to_unsigned_short(unsigned char ch1, unsigned char ch2);

//��ȡunsigned short�ĸߵ�λ
pair to_pair(unsigned short num);

unsigned int get_unsigned_int(FILE* fp);

unsigned short get_unsigned_short(FILE* fp);

void get_file_header(FILE* fp, Bitmap* pb);

void get_info_header(FILE* fp, Bitmap* pb);

Bitmap* loadBitmap(char* path);

void set_unsigned_short(FILE* fp, unsigned short num);

void set_unsigned_int(FILE* fp, unsigned short num);

void set_file_header(FILE* fp, Bitmap* pb);

void set_info_header(FILE* fp, Bitmap* pb);

void CopyBitmap(Bitmap* pb);
