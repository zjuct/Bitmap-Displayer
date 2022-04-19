#pragma once
#ifndef _JPG_H_
#define _JPG_H_

/*
by 陈桐
last_modified:2022-04-18 23:47
该头文件定义了JPG文件头
*/

//JPG文件头定义
//JPG头SOI1
//图像识别信息2
static struct APP0
{
    unsigned short int marker;            //段标识和类型FFE0
    unsigned short int length;            // 段长度 16
    unsigned char JFIFsignature[5];  // 存储ASCII码 "JFIF",'\0'
    unsigned char versionhi;         // 主版本号1
    unsigned char versionlo;         // 次版本号1
    unsigned char xyunits;           // 密度单位 0(无单位)
    unsigned short int xdensity;          // 水平方向的密度 1
    unsigned short int ydensity;          // 垂直方向的密度 1
    unsigned char thumbnwidth;       // 略缩图水平像素数目0
    unsigned char thumbnheight;      // 略缩图垂直像素数目0
}APP0info = { 0xFFE0,16,'J','F','I','F',0,1,1,0,1,1,0,0 };

//定义量化表3
static struct DQT {
    unsigned short int marker;   // 段标识和类型 0xFFDB
    unsigned short int length;   // 段长度
    unsigned char QTYinfo;  // 亮度QT值 00
    unsigned char Ytable[64];//内容
    unsigned char QTCbinfo; // 色度QT值 01
    unsigned char Cbtable[64];
} DQTinfo;

//图像基本信息SOF0 4
static struct  SOF0 {
    unsigned short int marker; //段标识和类型 0xFFC0
    unsigned short int length; // 段长度 17
    unsigned char precision;// 每个样本的位数
    unsigned short int height;   //图片高度
    unsigned short int width;     //图片宽度
    unsigned char nrofcomponents;//组件数量 3
    unsigned char IdY;  // 亮度= 1
    unsigned char HVY; // 采样系数 高位标识水平采样系数，低位表示垂直采样系数
    unsigned char QTY;  // 量化表号 0
    unsigned char IdCb; // 红色分量= 2
    unsigned char HVCb;
    unsigned char QTCb; // 1
    unsigned char IdCr; // 蓝色分量= 3
    unsigned char HVCr;
    unsigned char QTCr; // Normally equal to QTCb = 1
} SOF0info = { 0xFFC0,17,8,0,0,3,1,0x11,0,2,0x11,1,3,0x11,1 };

//定义huffman表 5
static struct DHT {
    unsigned short int marker;  //段标识和类型 0xFFC4
    unsigned short int length;  //段长度 19+n+....
    unsigned char HTYDCinfo; // 低4位HT号 第5位HT类型 高3位为0
    unsigned char YDC_codes[16]; //HT位表
    unsigned char YDC_values[12];// HT值表
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

//扫描行开始  6
static struct SOS {
    unsigned short int marker;  //段标识和类型 0xFFDA
    unsigned short int length; //长度 12
    unsigned char nrofcomponents; // 扫描行内组件数量 通常为3
    unsigned char IdY; //1
    unsigned char HTY; //低4位是AC表号，高4位是DC表号
    unsigned char IdCb; //2
    unsigned char HTCb; //0x11
    unsigned char IdCr; //3
    unsigned char HTCr; //0x11
    unsigned char Ss, Se, Bf; // not interesting, they should be 0,63,0
} SOSinfo = { 0xFFDA,12,3,1,0,2,0x11,3,0x11,0,0x3F,0 };


#endif