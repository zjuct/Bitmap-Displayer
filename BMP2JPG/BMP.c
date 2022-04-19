#pragma warning(disable:4996)
#include"BMP.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"JPG.h"

//�����ļ�ͷ��Ϣ�Ļ�����
unsigned char head_buffer[60];

//LoadBMP���ڽ�BMP�ļ��Ӵ����м��ص��ڴ��У�����ָ��Bitmap�ṹ���ָ��
Bitmap* LoadBMP(char* path) {
    RGBA lastcolor;   //�������հ�Ԫ��

    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        printf("�ļ���ʧ��");
        return NULL;
    }
    //��ʼ���ļ�ͷ������
    memset(head_buffer, 0, 60);

    //��ȡ�ļ�ͷ��Ϣ
    fread(head_buffer, 1, 54, fp);
    if ((head_buffer[0] != 'B') || (head_buffer[1] != 'M')) {
        printf("�ļ�����BMP��ʽ!\n");
        return NULL;
    }
    Bitmap* bmp = (Bitmap*)malloc(sizeof(Bitmap));
    if (bmp == NULL) {
        printf("�ڴ治��!\n");
        return NULL;
    }

    //�����ļ�ͷ
    GetFileHeader(bmp);
    GetInfoHeader(bmp);

    //��������8�������������
    if (bmp->info_header.bitmap_width % 8 != 0) {
        expendX = (bmp->info_header.bitmap_width / 8) * 8 + 8;
    }
    else {
        expendX = bmp->info_header.bitmap_width;
    }
    if (bmp->info_header.bitmap_height % 8 != 0) {
        expendY = (bmp->info_header.bitmap_height / 8) * 8 + 8;
    }
    else {
        expendY = bmp->info_header.bitmap_height;
    }
    bmp->expendX = expendX;
    bmp->expendY = expendY;

    if (bmp->info_header.image_depth == 32) {
        //����32λ��ɫ
        bmp->rgbacolor = (RGBA*)malloc(sizeof(RGBA) * expendX * expendY);
        if (bmp->rgbacolor == NULL) {
            printf("�ڴ治��!\n");
            free(bmp);
            return NULL;
        }

        int i = 0;
        for (;i < bmp->info_header.bitmap_height;++i) {
            fread(bmp->rgbacolor + i * expendX, 1, bmp->info_header.bitmap_width * 4, fp);
            //��ȡ���һ�����ص���ɫ
            memcpy(&lastcolor, bmp->rgbacolor + i * expendX + bmp->info_header.bitmap_width - 1, 4);
            int j = bmp->info_header.bitmap_width;
            //�����չ���ص���ɫ
            for (;j < expendX;++j) {
                memcpy(bmp->rgbacolor + i * expendX + bmp->info_header.bitmap_width, &lastcolor, 4);
            }
        }
        
        //���·�תͼƬ
        RGB* tmpline = malloc(sizeof(RGB) * expendX);
        if (tmpline == NULL) {
            printf("�ڴ治��!\n");
            free(bmp->rgbcolor);
            free(bmp);
            return NULL;
        }
        unsigned short top, buttom;
        for (top = 0, buttom = bmp->info_header.bitmap_height - 1;top < buttom;++top, --buttom) {
            memcpy(tmpline, bmp->rgbcolor + top * expendX, expendX * 3);
            memcpy(bmp->rgbcolor + top * expendX, bmp->rgbcolor + buttom * expendX, expendX * 3);
            memcpy(bmp->rgbcolor + buttom * expendX, tmpline, expendX * 3);
        }

        //�����չ��
        rgba_buffer = malloc(sizeof(RGBA) * expendX);
        if (rgba_buffer == NULL) {
            printf("�ڴ治��\n");
            free(bmp->rgbacolor);
            free(bmp);
            return NULL;
        }
        //�����һ�����rgba������
        memcpy(rgba_buffer, bmp->rgbacolor + (bmp->info_header.bitmap_height - 1) * expendX, expendX);
        int line = bmp->info_header.bitmap_height;
        for (;line < expendY;++line) {
            memcpy(bmp->rgbacolor + line * expendX, rgba_buffer, expendX);
        }

    }
    else if (bmp->info_header.image_depth == 24) {
        //����24λ��ɫ
        bmp->rgbcolor = (RGB*)malloc(sizeof(RGB) * expendX * expendY);
        if (bmp->rgbcolor == NULL) {
            printf("�ڴ治��!\n");
            free(bmp);
            return NULL;
        }
        unsigned char TOPBUF[256];
        unsigned char fillbytes;
        
        //�ж��������
        if (bmp->info_header.bitmap_width % 4 != 0)
            fillbytes = 4 - (bmp->info_header.bitmap_width % 4);
        else
            fillbytes = 0;
            
        for (int i = 0; i < bmp->info_header.bitmap_height; i++)
        {
            fread(bmp->rgbcolor + i * expendX, 1, bmp->info_header.bitmap_width * 3, fp);
            fread(TOPBUF, 1, fillbytes, fp);
            memcpy(&lastcolor, bmp->rgbcolor + i * expendX + bmp->info_header.bitmap_width - 1, 3);
            for (int j = bmp->info_header.bitmap_width; j < expendX; j++)
                memcpy(bmp->rgbcolor + i * expendX + j, &lastcolor, 3);
        }

        //�����չ��
        rgb_buffer = malloc(sizeof(RGB) * expendX);
        if (rgb_buffer == NULL) {
            printf("�ڴ治��\n");
            free(bmp->rgbcolor);
            free(bmp);
            return NULL;
        }
        //�����һ�����rgb������
        memcpy(rgb_buffer, bmp->rgbcolor + (bmp->info_header.bitmap_height - 1) * expendX, expendX * 3);
        int line = bmp->info_header.bitmap_height;
        for (;line < expendY;++line) {
            memcpy(bmp->rgbcolor + line * expendX, rgb_buffer, expendX * 3);
        }
        free(rgb_buffer);
        fclose(fp);
    }
    return bmp;
}

//��head_buffer��[begin,end]֮�������ת��Ϊunsigned int
unsigned int to_unsigned_int(int begin, int end) {
    unsigned int num = 0;
    while (begin <= end) {
        num = (num << 8) + head_buffer[end];
        --end;
    }
    return num;
}

//��head_buffer��[begin,end]֮�������ת��Ϊunsigned short
unsigned short to_unsigned_short(int begin, int end) {
    unsigned short num = 0;
    while (begin <= end) {
        num = (num << 8) + head_buffer[end];
        --end;
    }
    return num;
}

//��ȡbmp�ļ���Ϣ
void GetFileHeader(Bitmap* bmp) {
    bmp->file_header.file_type = to_unsigned_short(0, 1);
    bmp->file_header.file_size = to_unsigned_int(2, 5);
    bmp->file_header.reserved1 = to_unsigned_short(6, 7);
    bmp->file_header.reserved2 = to_unsigned_short(8, 9);
    bmp->file_header.offset_bits = to_unsigned_int(10, 13);
}

//��ȡλͼ��Ϣ
void GetInfoHeader(Bitmap* bmp) {
    bmp->info_header.bitmap_info_size = to_unsigned_int(14, 17);
    bmp->info_header.bitmap_width = to_unsigned_int(18, 21);
    bmp->info_header.bitmap_height = to_unsigned_int(22, 25);
    bmp->info_header.planes = to_unsigned_short(26, 27);
    bmp->info_header.image_depth = to_unsigned_short(28, 29);
    bmp->info_header.compression = to_unsigned_short(30, 33);
    bmp->info_header.image_size = to_unsigned_int(34, 37);
    bmp->info_header.x_pels_permeter = to_unsigned_int(38, 41);
    bmp->info_header.y_pels_permeter = to_unsigned_int(42, 45);
    bmp->info_header.color_used = to_unsigned_short(46, 49);
    bmp->info_header.color_important = to_unsigned_short(50, 53);
}

//д��unsigned_short
void set_unsigned_short(FILE* fp, unsigned short num) {
    for (int i = 0;i < 2;i++) {
        unsigned char ch = num % (0x1 << 8);
        num /= (0x1 << 8);
        fputc(ch, fp);
    }
}

//д��unsigned_int
void set_unsigned_int(FILE* fp, unsigned int num) {
    for (int i = 0;i < 4;i++) {
        unsigned char ch = num % (0x1 << 8);
        num /= (0x1 << 8);
        fputc(ch, fp);
    }
}

//�����ļ�ͷ
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

//SaveBMP��Bitmap�ṹ�屣�浽������
void SaveBMP(char* path, Bitmap* bmp) {
    FILE* fp = fopen(path, "wb");
    if (fp == NULL) {
        printf("�ļ���ʧ��!\n");
        return;
    }
    set_file_header(fp, bmp);
    set_info_header(fp, bmp);
    //����32λ��ɫ
    if (bmp->info_header.image_depth == 32) {
        int i = 0;
        for (;i <= bmp->info_header.bitmap_height;++i) {
            int j = 0;
            for (;j < bmp->info_header.bitmap_width;++j) {
                fputc(bmp->rgbacolor[i * expendX + j].R, fp);
                fputc(bmp->rgbacolor[i * expendX + j].G, fp);
                fputc(bmp->rgbacolor[i * expendX + j].B, fp);
                fputc(bmp->rgbacolor[i * expendX + j].A, fp);
            }
        }
    }
    else if (bmp->info_header.image_depth == 24) {
        //����24λ��ɫ
        int i = 0;
        for (;i <= bmp->info_header.bitmap_height;++i) {
            int j = 0;
            for (;j < bmp->info_header.bitmap_width;++j) {
                fputc(bmp->rgbcolor[i * expendX + j].R, fp);
                fputc(bmp->rgbcolor[i * expendX + j].G, fp);
                fputc(bmp->rgbcolor[i * expendX + j].B, fp);
            }
        }
    }
}