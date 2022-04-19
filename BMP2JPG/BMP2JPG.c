#pragma warning(disable:4996)
#include"JPG.h"
#include"HuffmanCode.h"
#include"Qualization.h"
#include"util.h"
#include"BMP.h"
#include"BMP2JPG.h"

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void DCT_Qua_ZigZag(signed char* matrix, unsigned char* table);
void DCT_Qua(signed char data[8][8], unsigned char* fdtbl, signed short int* outdata);


static double DCT_Y[8][8];
static double DCT_Cb[8][8];
static double DCT_Cr[8][8];

static signed short int QuaRes[64];

static unsigned int expendX;
static unsigned int expendY;

void Bmp2Jpg(char* bmp_path, char* jpg_path) {
    Bitmap* bmp = LoadBMP(bmp_path);

    SOF0info.width = bmp->info_header.bitmap_width;
    SOF0info.height = bmp->info_header.bitmap_height;

    expendX = bmp->expendX;
    expendY = bmp->expendY;
    InitJpgHead();
    WriteJpgHead(jpg_path);
    Encode(bmp);
}

void InitJpgHead() {
    int i = 0;

    //初始化量化表
    DQTinfo.marker = 0XFFDB;
    DQTinfo.length = 132;
    DQTinfo.QTYinfo = 0;
    DQTinfo.QTCbinfo = 1;
    for (i = 0; i < 64; ++i)
    {
        DQTinfo.Ytable[i] = DQT_Y[i];
        DQTinfo.Cbtable[i] = DQT_C[i];
    }
    //初始化huffman表
    DHTinfo.marker = 0xFFC4;
    DHTinfo.length = 0x01A2;
    DHTinfo.HTYDCinfo = 0; //0号直流表
    for (i = 0;i < 16;i++)
        DHTinfo.YDC_codes[i] = dc_y_codes[i + 1];
    for (i = 0;i <= 11;i++)
        DHTinfo.YDC_values[i] = dc_y_values[i];

    DHTinfo.HTYACinfo = 0x10; //0号交流表
    for (i = 0;i < 16;i++)
        DHTinfo.YAC_codes[i] = ac_y_codes[i + 1];
    for (i = 0;i <= 161;i++)
        DHTinfo.YAC_values[i] = ac_y_values[i];
    DHTinfo.HTCbDCinfo = 1; //1号直流表

    for (i = 0;i < 16;i++)
        DHTinfo.CbDC_codes[i] = dc_cb_codes[i + 1];
    for (i = 0;i <= 11;i++)
        DHTinfo.CbDC_values[i] = dc_cb_values[i];
    DHTinfo.HTCbACinfo = 0x11; //1号交流表

    for (i = 0;i < 16;i++)
        DHTinfo.CbAC_codes[i] = ac_cb_codes[i + 1];
    for (i = 0;i <= 161;i++)
        DHTinfo.CbAC_values[i] = ac_cb_values[i];
}

//写入JPG文件头
void WriteJpgHead(char* path) {
    fp_jpg = fopen(path, "wb");
    if (fp_jpg == NULL) {
        printf("文件打开失败!\n");
        exit(0);
    }

    unsigned char i;
    //1.SOI
    writeword(0xFFD8)
    //2.APP0
    writeword(APP0info.marker);
    writeword(APP0info.length);
    writebyte('J');
    writebyte('F');
    writebyte('I');
    writebyte('F');
    writebyte(0);
    writebyte(APP0info.versionhi);
    writebyte(APP0info.versionlo);
    writebyte(APP0info.xyunits);
    writeword(APP0info.xdensity);
    writeword(APP0info.ydensity);
    writebyte(APP0info.thumbnwidth);
    writebyte(APP0info.thumbnheight);
    //3.DQT
    writeword(DQTinfo.marker);
    writeword(DQTinfo.length);
    writebyte(DQTinfo.QTYinfo);
    for (i = 0;i < 64;i++)
        writebyte(DQTinfo.Ytable[i]);
    writebyte(DQTinfo.QTCbinfo);
    for (i = 0;i < 64;i++)
        writebyte(DQTinfo.Cbtable[i]);
    //4.SOF0
    writeword(SOF0info.marker);
    writeword(SOF0info.length);
    writebyte(SOF0info.precision);
    writeword(SOF0info.height);
    writeword(SOF0info.width);
    writebyte(SOF0info.nrofcomponents);
    writebyte(SOF0info.IdY);
    writebyte(SOF0info.HVY);
    writebyte(SOF0info.QTY);
    writebyte(SOF0info.IdCb);
    writebyte(SOF0info.HVCb);
    writebyte(SOF0info.QTCb);
    writebyte(SOF0info.IdCr);
    writebyte(SOF0info.HVCr);
    writebyte(SOF0info.QTCr);
    //5.DHT
    writeword(DHTinfo.marker);
    writeword(DHTinfo.length);
    writebyte(DHTinfo.HTYDCinfo);
    for (i = 0;i < 16;i++)
        writebyte(DHTinfo.YDC_codes[i]);
    for (i = 0;i <= 11;i++)
        writebyte(DHTinfo.YDC_values[i]);
    writebyte(DHTinfo.HTYACinfo);
    for (i = 0;i < 16;i++)
        writebyte(DHTinfo.YAC_codes[i]);
    for (i = 0;i <= 161;i++)
        writebyte(DHTinfo.YAC_values[i]);
    writebyte(DHTinfo.HTCbDCinfo);
    for (i = 0;i < 16;i++)
        writebyte(DHTinfo.CbDC_codes[i]);
    for (i = 0;i <= 11;i++)
        writebyte(DHTinfo.CbDC_values[i]);
    writebyte(DHTinfo.HTCbACinfo);
    for (i = 0;i < 16;i++)
        writebyte(DHTinfo.CbAC_codes[i]);
    for (i = 0;i <= 161;i++)
        writebyte(DHTinfo.CbAC_values[i]);
    //6.SOS
    writeword(SOSinfo.marker);
    writeword(SOSinfo.length);
    writebyte(SOSinfo.nrofcomponents);
    writebyte(SOSinfo.IdY);
    writebyte(SOSinfo.HTY);
    writebyte(SOSinfo.IdCb);
    writebyte(SOSinfo.HTCb);
    writebyte(SOSinfo.IdCr);
    writebyte(SOSinfo.HTCr);
    writebyte(SOSinfo.Ss);
    writebyte(SOSinfo.Se);
    writebyte(SOSinfo.Bf);
}

void Encode(Bitmap* bmp) {
    //huffman编码表
    bitstring YDC[12];
    bitstring CbDC[12];
    bitstring YAC[256];
    bitstring CbAC[256];

    //计算各项的huffman表
    InitHuffmanCode(dc_y_codes, dc_y_values, YDC);
    InitHuffmanCode(dc_cb_codes, dc_cb_values, CbDC);
    InitHuffmanCode(ac_y_codes, ac_y_values, YAC);
    InitHuffmanCode(ac_cb_codes, ac_cb_values, CbAC);

    leng_and_numbercode();

    bitstring fillbit;
    signed short int DCY = 0, DCCb = 0, DCCr = 0;//直流系数

    unsigned short int i, j;
    //8*8为一组
    for (i = 0;i < expendY;i += 8) {
        int q = 0;
        for (j = 0;j < expendX;j += 8) {
            //将RGB颜色空间转换为YCbCr颜色空间
            RGB2YcbCr(bmp, i, j);

            //对三个YCbCr矩阵进行离散余弦变换DCT
            DCT_Qua_ZigZag(YDU,DQTinfo.Ytable);

            //DCT(YDU, DCT_Y);
            
            //Qualization(DCT_Y, DQTinfo.Ytable, QuaRes);  //量化
            //ZigZagTraverse(QuaRes);  //Zig-Zag遍历转化为一维数组
            HuffmanEncode(&DCY, YDC, YAC);

            DCT_Qua_ZigZag(CbDU, DQTinfo.Cbtable);
            //DCT(CbDU, DCT_Cb);
            //Qualization(DCT_Cb, DQTinfo.Cbtable, QuaRes);
            //ZigZagTraverse(QuaRes);
            HuffmanEncode(&DCCb, CbDC, CbAC);

            DCT_Qua_ZigZag(CrDU, DQTinfo.Cbtable);
            //DCT(CrDU, DCT_Cr);
            //Qualization(DCT_Cr, DQTinfo.Cbtable, QuaRes);
            //ZigZagTraverse(QuaRes);
            HuffmanEncode(&DCCr, CbDC, CbAC);

            //Huffman编码
        }
    }
    if (bytepos > 0) {
        fillbit.length = bytepos;
        fillbit.value = 0;
        writefile(fillbit);
    }
    writeword(0xffd9);
    
}

void InitHuffmanCode(unsigned char* codes, unsigned char* values, bitstring* HT) {
    unsigned char k, j;
    unsigned char pos = 0;
    unsigned short int codevalue = 0;
    for (k = 1; k <= 16; ++k)
    {
        for (j = 1;j <= codes[k]; ++j)
        {
            HT[values[pos]].value = codevalue; //编码值
            HT[values[pos]].length = k;     //长度
            pos++;
            codevalue++;
        }
        codevalue *= 2;
    }
}

void leng_and_numbercode() {
    signed long int n;
    signed long int one = 1, two = 2;
    unsigned char leng;

    //开辟空间
    code_u = (bitstring*)malloc(65535 * sizeof(bitstring));
    if (code_u == NULL)
    {
        printf("存储空间开辟失败");
        exit(0);
    }
    code_d = code_u + 32767;
    for (leng = 1; leng <= 15; ++leng)
    {
        for (n = one; n < two; ++n)
        {
            code_d[n].length = leng;
            code_d[n].value = (unsigned short int)n;
        }

        for (n = -(two - 1); n <= -one; ++n)
        {
            code_d[n].length = leng;
            code_d[n].value = (unsigned short int)(two - 1 + n);
        }
        two <<= 1;
        one <<= 1;
    }
}


void RGB2YcbCr(Bitmap* bmp, unsigned short int y,unsigned short int x) {
    
    unsigned char pos = 0;
    unsigned char R, G, B;
    if (bmp->info_header.image_depth == 24) {
        for (int i = y;i < y + 8;++i) {
            for (int j = x;j < x + 8;++j) {
                
                R = bmp->rgbcolor[i * expendX + j].R;
                G = bmp->rgbcolor[i * expendX + j].G;
                B = bmp->rgbcolor[i * expendX + j].B;
                YDU[pos] = (signed char)(((signed long int)(65536 * 0.299 + 0.5) * R + (signed long int)(65536 * 0.587 + 0.5) * G + (signed long int)(65536 * 0.114 + 0.5) * B) >> 16) - 128;
                CbDU[pos] = (signed char)(((signed long int)(65536 * -0.16874 + 0.5) * R + (signed long int)(65536 * -0.33726 + 0.5) * G + (signed long int)(65536 * 0.5 + 0.5) * B) >> 16);
                CrDU[pos] = (signed char)(((signed long int)(65536 * 0.5 + 0.5) * R + (signed long int)(65536 * -0.41869 + 0.5) * G + (signed long int)(65536 * -0.08131 + 0.5) * B) >> 16);
                pos++;
            }
        }
    }
    else if (bmp->info_header.image_depth == 32) {
        for (int i = y;i < y + 8;++i) {
            for (int j = x;j < x + 8;++j) {
                R = bmp->rgbacolor[i * expendX + j].R;
                G = bmp->rgbacolor[i * expendX + j].G;
                B = bmp->rgbacolor[i * expendX + j].B;
                YDU[pos] = (signed char)(((signed long int)(65536 * 0.299 + 0.5) * R + (signed long int)(65536 * 0.587 + 0.5) * G + (signed long int)(65536 * 0.114 + 0.5) * B) >> 16) - 128;
                CbDU[pos] = (signed char)(((signed long int)(65536 * -0.16874 + 0.5) * R + (signed long int)(65536 * -0.33726 + 0.5) * G + (signed long int)(65536 * 0.5 + 0.5) * B) >> 16);
                CrDU[pos] = (signed char)(((signed long int)(65536 * 0.5 + 0.5) * R + (signed long int)(65536 * -0.41869 + 0.5) * G + (signed long int)(65536 * -0.08131 + 0.5) * B) >> 16);
                pos++;
            }
        }
    }
}

void DCT(signed char* matrix, double(*res)[8]) {

    double ALPHA, BETA;
    short u = 0;
    short v = 0;
    short i = 0;
    short j = 0;
    for (u = 0; u < 8; u++)
    {
        for (v = 0; v < 8; v++)
        {
            if (u == 0)
                ALPHA = sqrt(1.0 / 8);
            else
                ALPHA = sqrt(2.0 / 8);
            if (v == 0)
                BETA = sqrt(1.0 / 8);
            else
                BETA = sqrt(2.0 / 8);
            float tmp = 0.0;
            for (i = 0; i < 8; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    tmp += matrix[i * 8 + j] * cos((2 * i + 1) * u * PI / (2.0 * 8)) * cos((2 * j + 1) * v * PI / (2.0 * 8));
                }
            }
            res[u][v] = ALPHA * BETA * tmp;
        }
    }
}

void Qualization(double(*matrix)[8], unsigned char* table,signed short int* res) {

    float temp = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            temp = matrix[i][j] / (float)((double)table[i * 8 + j]);
            res[i * 8 + j] = (signed short int) ((signed short int)(temp + 16384.5) - 16384);
        }

}

//Zig-Zag遍历矩阵
void ZigZagTraverse(signed short* matrix) {
    for (int i = 0; i <= 63; i++)
        ZigZagBuffer[ZigZagCode[i]] = matrix[i];
}

void HuffmanEncode(signed short int* DC, bitstring* DCtable, bitstring* ACtable)
{
    signed short int diff;
    unsigned char endpos, startpos, i = 1, nz;

    diff = ZigZagBuffer[0] - *DC; //差值
    *DC = ZigZagBuffer[0];
    //写入直流值
    if (diff == 0)
        writefile(DCtable[0]); //写入0的huffman编码
    else
    {
        writefile(DCtable[code_d[diff].length]);//写入diff长度的huffman编码
        writefile(code_d[diff]); //写入二进制编码
    }
    //写入交流值
    for (endpos = 63; (endpos > 0) && (ZigZagBuffer[endpos] == 0); --endpos);
    if (endpos == 0)
    {
        writefile(ACtable[0x00]);
        return;
    }
    while (i <= endpos)
    {
        startpos = i;
        for (; (ZigZagBuffer[i] == 0) && (i <= endpos); ++i);
        nz = i - startpos;
        for (;nz >= 16; nz -= 16)//长度大于16需要分多块
            writefile(ACtable[0xf0]);
        writefile(ACtable[nz * 16 + code_d[ZigZagBuffer[i]].length]);
        writefile(code_d[ZigZagBuffer[i]]);
        ++i;
    }
    if (endpos != 63)
        writefile(ACtable[0x00]);   //EOB
}


void writefile(bitstring a)
{
    unsigned short int values = a.value;
    signed char leng_a = a.length;
    while (leng_a > 0)
    {
        if (values & mark[leng_a - 1]) //判断对应位置是否为1
            bytecode |= mark[bytepos - 1];//当前最高位至1
        leng_a--;
        bytepos--;
        if (bytepos <= 0)
        {
            if (bytecode == 0xff)
            {
                writebyte(0xff);
                writebyte(0x00);//说明ff不是段标志
            }
            else
                writebyte(bytecode);
            bytecode = 0;
            bytepos = 8;
        }
    }
}


void DCT_Qua_ZigZag(signed char* matrix,unsigned char* table) {
    float temp;
    signed short int D1[64];
    signed char mdata[8][8];
    unsigned char i, j;
    //离散余弦变换
    for (i = 0; i < 8; ++i)
        for (j = 0;j < 8; ++j)
            mdata[i][j] = matrix[8 * i + j];
    DCT_Qua(mdata, table, D1);
    //z型编码
    for (i = 0; i <= 63; i++)
        ZigZagBuffer[ZigZagCode[i]] = D1[i];
}

void DCT_Qua(signed char data[8][8], unsigned char* fdtbl, signed short int* outdata) {
    float output[8][8], temp;
    double ALPHA, BETA;
    short u = 0;
    short v = 0;
    short i = 0;
    short j = 0;
    for (u = 0; u < 8; u++)
    {
        for (v = 0; v < 8; v++)
        {
            if (u == 0)
                ALPHA = sqrt(1.0 / 8);
            else
                ALPHA = sqrt(2.0 / 8);
            if (v == 0)
                BETA = sqrt(1.0 / 8);
            else
                BETA = sqrt(2.0 / 8);
            float tmp = 0.0;
            for (i = 0; i < 8; i++)
            {
                for (j = 0; j < 8; j++)
                {
                    tmp += data[i][j] * cos((2 * i + 1) * u * PI / (2.0 * 8)) * cos((2 * j + 1) * v * PI / (2.0 * 8));
                }
            }
            output[u][v] = ALPHA * BETA * tmp;
        }
    }

    // 量化
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
        {
            temp = output[i][j] / (float)((double)fdtbl[ZigZagCode[i * 8 + j]]);
            outdata[i * 8 + j] = (signed short int) ((signed short int)(temp + 16384.5) - 16384);
        }
}