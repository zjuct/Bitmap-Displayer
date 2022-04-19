#pragma once
#ifndef _JPG_H_
#define _JPG_H_

/*
by ��ͩ
last_modified:2022-04-18 23:47
��ͷ�ļ�������JPG�ļ�ͷ
*/

//JPG�ļ�ͷ����
//JPGͷSOI1
//ͼ��ʶ����Ϣ2
static struct APP0
{
    unsigned short int marker;            //�α�ʶ������FFE0
    unsigned short int length;            // �γ��� 16
    unsigned char JFIFsignature[5];  // �洢ASCII�� "JFIF",'\0'
    unsigned char versionhi;         // ���汾��1
    unsigned char versionlo;         // �ΰ汾��1
    unsigned char xyunits;           // �ܶȵ�λ 0(�޵�λ)
    unsigned short int xdensity;          // ˮƽ������ܶ� 1
    unsigned short int ydensity;          // ��ֱ������ܶ� 1
    unsigned char thumbnwidth;       // ����ͼˮƽ������Ŀ0
    unsigned char thumbnheight;      // ����ͼ��ֱ������Ŀ0
}APP0info = { 0xFFE0,16,'J','F','I','F',0,1,1,0,1,1,0,0 };

//����������3
static struct DQT {
    unsigned short int marker;   // �α�ʶ������ 0xFFDB
    unsigned short int length;   // �γ���
    unsigned char QTYinfo;  // ����QTֵ 00
    unsigned char Ytable[64];//����
    unsigned char QTCbinfo; // ɫ��QTֵ 01
    unsigned char Cbtable[64];
} DQTinfo;

//ͼ�������ϢSOF0 4
static struct  SOF0 {
    unsigned short int marker; //�α�ʶ������ 0xFFC0
    unsigned short int length; // �γ��� 17
    unsigned char precision;// ÿ��������λ��
    unsigned short int height;   //ͼƬ�߶�
    unsigned short int width;     //ͼƬ���
    unsigned char nrofcomponents;//������� 3
    unsigned char IdY;  // ����= 1
    unsigned char HVY; // ����ϵ�� ��λ��ʶˮƽ����ϵ������λ��ʾ��ֱ����ϵ��
    unsigned char QTY;  // ������� 0
    unsigned char IdCb; // ��ɫ����= 2
    unsigned char HVCb;
    unsigned char QTCb; // 1
    unsigned char IdCr; // ��ɫ����= 3
    unsigned char HVCr;
    unsigned char QTCr; // Normally equal to QTCb = 1
} SOF0info = { 0xFFC0,17,8,0,0,3,1,0x11,0,2,0x11,1,3,0x11,1 };

//����huffman�� 5
static struct DHT {
    unsigned short int marker;  //�α�ʶ������ 0xFFC4
    unsigned short int length;  //�γ��� 19+n+....
    unsigned char HTYDCinfo; // ��4λHT�� ��5λHT���� ��3λΪ0
    unsigned char YDC_codes[16]; //HTλ��
    unsigned char YDC_values[12];// HTֵ��
    unsigned char HTYACinfo; // = 0x10
    unsigned char YAC_codes[16];
    unsigned char YAC_values[162];
    unsigned char HTCbDCinfo; // = 1
    unsigned char CbDC_codes[16];
    unsigned char CbDC_values[12];
    unsigned char HTCbACinfo; //  = 0x11
    unsigned char CbAC_codes[16];
    unsigned char CbAC_values[162];
} DHTinfo;

//ɨ���п�ʼ  6
static struct SOS {
    unsigned short int marker;  //�α�ʶ������ 0xFFDA
    unsigned short int length; //���� 12
    unsigned char nrofcomponents; // ɨ������������� ͨ��Ϊ3
    unsigned char IdY; //1
    unsigned char HTY; //��4λ��AC��ţ���4λ��DC���
    unsigned char IdCb; //2
    unsigned char HTCb; //0x11
    unsigned char IdCr; //3
    unsigned char HTCr; //0x11
    unsigned char Ss, Se, Bf; // not interesting, they should be 0,63,0
} SOSinfo = { 0xFFDA,12,3,1,0,2,0x11,3,0x11,0,0x3F,0 };


#endif