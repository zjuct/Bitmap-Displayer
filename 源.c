#include"graphics.h"
#include"extgraph.h"

#include<Windows.h>
#include<conio.h>
#include<commdlg.h>
#include<ShlObj.h>
#include"Bitmap.h"

#define MenuItem1  1001
#define MenuItem2  1002
#define FilePopItem1 2001
#define FilePopItem2 2002
#define FilePopItem3 2003
#define FilePopItem4 2004
#define EditPopItem1 3001
#define EditPopItem2 3002
#define EditPopItem3 3003
#define HelpPopItem1 4001
#define HelpPopItem2 4002

#define StatusWindow 5001

char path[100];
Bitmap* pb;

void InitLayout();
void drawBitmap(Bitmap* pb, HWND hwnd);

void MyKeyboardEventCallback(int key, int event) {

}

void MyCharEventCallback(char c) {

}

void MyMouseEventCallback(int x, int y, int button, int event) {

}

void MyTimerEventCallback(int timerID) {

}

void MySizeEventCallback() {
	if (pb) {
		drawBitmap(pb, GetActiveWindow());
	}
}

void MyCommandEventCallback(int id) {
	switch (id) {
	case FilePopItem1:
	{
		//弹出文件对话框
		TCHAR strFilename[MAX_PATH] = { 0 };
		TCHAR strFileTitle[MAX_PATH] = { 0 };
		OPENFILENAME ofn;
		static TCHAR szFilter[] = TEXT("BMP Files (*.BMP)\0*.bmp\0")  \
			TEXT("JPEG Files (*.JPG)\0*.jpg\0") \
			TEXT("All Files (*.*)\0*.*\0\0");

		ofn.lStructSize = sizeof(OPENFILENAME);		//结构体大小
		ofn.hwndOwner = GetActiveWindow();			//指定窗口句柄则为模态，为NULL则为非模态
		ofn.hInstance = NULL;
		ofn.lpstrFilter = szFilter;					//过滤器
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = strFilename;				//返回接收的全路径文件名
		ofn.nMaxFile = MAX_PATH;					//缓冲区大小
		ofn.lpstrFileTitle = strFileTitle;		    //返回接收的文件名
		ofn.nMaxFileTitle = MAX_PATH;			    //缓冲区大小
		ofn.lpstrInitialDir = NULL;				    //初始目录为默认
		ofn.lpstrTitle = NULL;						//对话框标题
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;    //设置属性
		ofn.nFileOffset = 0;						//文件名在字符串中的起始位置
		ofn.nFileExtension = 0;						//扩展名在字符串中的起始位置
		ofn.lpstrDefExt = TEXT("bmp");			    //默认扩展名
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;

		if (GetOpenFileName(&ofn))
		{
			//将文件全路径拷贝到全局变量中
			strcpy(path, ofn.lpstrFile);
		}
		//创建Bitmap对象
		pb = loadBitmap(path);
		//绘制位图
		drawBitmap(pb, GetActiveWindow());
	}
	return;
	}
}

void Main() {
	InitGraphics();
	//界面初始化
	InitLayout();

	//注册回调函数
	registerMouseEvent(MyMouseEventCallback);
	registerCharEvent(MyCharEventCallback);
	registerKeyboardEvent(MyKeyboardEventCallback);
	registerTimerEvent(MyTimerEventCallback);
	registerCommandEvent(MyCommandEventCallback);
	registerSizeEvent(MySizeEventCallback);
}

void InitLayout() {
	//设置窗口标题
	SetWindowTitle("图片查看器");

	//创建顶层菜单
	HMENU hmenu = CreateMenu();	

	//创建文件菜单的弹出式菜单项
	HMENU file_pop_menu = CreatePopupMenu();
	AppendMenu(file_pop_menu, 0, FilePopItem1, "打开");
	AppendMenu(file_pop_menu, 0, FilePopItem2, "保存");
	AppendMenu(file_pop_menu, 0, FilePopItem3, "另存为");
	AppendMenu(file_pop_menu, 0, FilePopItem4, "关闭");
	//将文件菜单绑定到菜单栏上
	AppendMenu(hmenu, MF_POPUP, file_pop_menu,"文件");

	//创建编辑菜单的弹出式菜单项
	HMENU edit_pop_menu = CreatePopupMenu();
	AppendMenu(edit_pop_menu, 0, EditPopItem1, "旋转");
	AppendMenu(edit_pop_menu, 0, EditPopItem2, "缩放");
	AppendMenu(edit_pop_menu, 0, EditPopItem3, "裁剪");	
	//将编辑菜单绑定到菜单栏上
	AppendMenu(hmenu, MF_POPUP, edit_pop_menu, "编辑");

	//创建帮助菜单的弹出式菜单项
	HMENU help_pop_menu = CreatePopupMenu();
	AppendMenu(help_pop_menu, 0, HelpPopItem1, "查看帮助");
	AppendMenu(help_pop_menu, 0, HelpPopItem2, "关于图片查看器");
	//将帮助菜单绑定到菜单栏上
	AppendMenu(hmenu, MF_POPUP, help_pop_menu, "帮助");

	//将顶层菜单绑定到当前窗口
	SetMenu(GetActiveWindow(), hmenu);

	//创建状态栏
	//CreateWindow(STATUSCLASSNAME, "状态栏", SBS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		//0, 0, 0, 0, GetActiveWindow(), StatusWindow, GethInstance(), NULL);

	//CreateStatusWindow(SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, "状态栏", GetActiveWindow(), StatusWindow);
}

void drawBitmap(Bitmap* pb, HWND hwnd) {

	HDC hdc = GetDC(hwnd);
	//创建与设备DC相兼容的内存DC
	HDC hdcmem = CreateCompatibleDC(hdc);
	/*
	HBITMAP hBitmap = (HBITMAP)LoadImage(
		NULL,
		path,
		IMAGE_BITMAP,
		0, 0,
		LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		*/

	//将内存DC绑定到位图对象
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, pb->info_header.bitmap_width, pb->info_header.bitmap_width);
	HBITMAP h1 = SelectObject(hdcmem, bitmap);
	
	//在内存DC上绘制
	for (int i = 0;i < pb->info_header.bitmap_height;i++) {
		for (int j = 0;j < pb->info_header.bitmap_width;j++) {
			SetPixel(hdcmem, i , j , RGB(pb->color[i * pb->info_header.bitmap_width + j].r,
				pb->color[i * pb->info_header.bitmap_width + j].g,
				pb->color[i * pb->info_header.bitmap_width + j].b));
		}
	}

	//获取当前窗口大小
	RECT rect;
	GetWindowRect(GetActiveWindow(), &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	//将内存DC中的内容拷贝到hdc
	BitBlt(hdc, width / 2 - pb->info_header.bitmap_width / 2, height / 2 - pb->info_header.bitmap_height / 2, 
		pb->info_header.bitmap_width, pb->info_header.bitmap_height, hdcmem, 0, 0, SRCCOPY);
}