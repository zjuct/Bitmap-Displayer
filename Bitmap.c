#include"Bitmap.h"
#include<Windows.h>

//将两个char拼接为unsigned short
unsigned short to_unsigned_short(unsigned char ch1, unsigned char ch2) {
	unsigned short num = ch2;
	num = (num << 8) + ch1;
	return num;
}

//提取unsigned short的高低位
pair to_pair(unsigned short num) {
	pair p;
	//高位
	p.ch1 = (unsigned char)(num / (0x1 << 8));
	//底位
	p.ch2 = (unsigned char)(num % (0x1 << 8));
	return p;
}

unsigned int get_unsigned_int(FILE* fp) {
	unsigned char ch1 = fgetc(fp);
	unsigned char ch2 = fgetc(fp);
	unsigned char ch3 = fgetc(fp);
	unsigned char ch4 = fgetc(fp);
	unsigned int num = ((unsigned int)ch4) << 24;
	num += ((unsigned int)ch3) << 16;
	num += ((unsigned int)ch2) << 8;
	num += ((unsigned int)ch1);
	return num;
}

unsigned short get_unsigned_short(FILE* fp) {
	unsigned char ch1 = fgetc(fp);
	unsigned char ch2 = fgetc(fp);
	unsigned short num = (((unsigned short)ch2) << 8) + (unsigned short)ch1;
	return num;
}

void get_file_header(FILE* fp, Bitmap* pb) {
	pb->file_header.file_type = get_unsigned_short(fp);
	pb->file_header.file_size = get_unsigned_int(fp);
	pb->file_header.reserved1 = get_unsigned_short(fp);
	pb->file_header.reserved2 = get_unsigned_short(fp);
	pb->file_header.offset_bits = get_unsigned_int(fp);
}

void get_info_header(FILE* fp, Bitmap* pb) {
	pb->info_header.bitmap_info_size = get_unsigned_int(fp);
	pb->info_header.bitmap_width = get_unsigned_int(fp);
	pb->info_header.bitmap_height = get_unsigned_int(fp);
	pb->info_header.planes = get_unsigned_short(fp);
	pb->info_header.image_depth = get_unsigned_short(fp);
	pb->info_header.compression = get_unsigned_int(fp);
	pb->info_header.image_size = get_unsigned_int(fp);
	pb->info_header.x_pels_permeter = get_unsigned_int(fp);
	pb->info_header.y_pels_permeter = get_unsigned_int(fp);
	pb->info_header.color_used = get_unsigned_int(fp);
	pb->info_header.color_important = get_unsigned_int(fp);
}

Bitmap* loadBitmap(char* path) {
	FILE* fp;
	if (!(fp = fopen(path, "rb"))) {
		//printf("文件打开失败\n");
		exit(0);
	}
	Bitmap* pb = (Bitmap*)malloc(sizeof(Bitmap));
	get_file_header(fp, pb);
	get_info_header(fp, pb);

	unsigned char ch;
	pb->pos = 0;
	while (!feof(fp)) {
		pb->color[pb->pos].r = fgetc(fp);
		pb->color[pb->pos].g = fgetc(fp);
		pb->color[pb->pos].b = fgetc(fp);
		pb->pos++;
	}
	fclose(fp);
	return pb;
}

void set_unsigned_short(FILE* fp, unsigned short num) {
	for (int i = 0;i < 2;i++) {
		unsigned char ch = num % (0x1 << 8);
		num /= (0x1 << 8);
		fputc(ch, fp);
	}
}

void set_unsigned_int(FILE* fp, unsigned short num) {
	for (int i = 0;i < 4;i++) {
		unsigned char ch = num % (0x1 << 8);
		num /= (0x1 << 8);
		fputc(ch, fp);
	}
}

void set_file_header(FILE* fp, Bitmap* pb) {
	set_unsigned_short(fp, pb->file_header.file_type);
	set_unsigned_int(fp, pb->file_header.file_size);
	set_unsigned_short(fp, pb->file_header.reserved1);
	set_unsigned_short(fp, pb->file_header.reserved2);
	set_unsigned_int(fp, pb->file_header.offset_bits);
}

void set_info_header(FILE* fp, Bitmap* pb) {
	set_unsigned_int(fp, pb->info_header.bitmap_info_size);
	set_unsigned_int(fp, pb->info_header.bitmap_width);
	set_unsigned_int(fp, pb->info_header.bitmap_height);
	set_unsigned_short(fp, pb->info_header.planes);
	set_unsigned_short(fp, pb->info_header.image_depth);
	set_unsigned_int(fp, pb->info_header.compression);
	set_unsigned_int(fp, pb->info_header.image_size);
	set_unsigned_int(fp, pb->info_header.x_pels_permeter);
	set_unsigned_int(fp, pb->info_header.y_pels_permeter);
	set_unsigned_int(fp, pb->info_header.color_used);
	set_unsigned_int(fp, pb->info_header.color_important);
}

void CopyBitmap(Bitmap* pb) {
	FILE* fp;
	char path[MAX_SIZE] = "E:\\file\\098.bmp";
	//scanf("%s", path);
	if (!(fp = fopen(path, "wb"))) {
		printf("文件打开失败\n");
		exit(0);
	}
	set_file_header(fp, pb);
	set_info_header(fp, pb);
	for (int i = 0;i < pb->pos;i++) {
		fputc(pb->color[i].r, fp);
		fputc(pb->color[i].g, fp);
		fputc(pb->color[i].b, fp);
	}
	fclose(fp);
}