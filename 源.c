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
		//�����ļ��Ի���
		TCHAR strFilename[MAX_PATH] = { 0 };
		TCHAR strFileTitle[MAX_PATH] = { 0 };
		OPENFILENAME ofn;
		static TCHAR szFilter[] = TEXT("BMP Files (*.BMP)\0*.bmp\0")  \
			TEXT("JPEG Files (*.JPG)\0*.jpg\0") \
			TEXT("All Files (*.*)\0*.*\0\0");

		ofn.lStructSize = sizeof(OPENFILENAME);		//�ṹ���С
		ofn.hwndOwner = GetActiveWindow();			//ָ�����ھ����Ϊģ̬��ΪNULL��Ϊ��ģ̬
		ofn.hInstance = NULL;
		ofn.lpstrFilter = szFilter;					//������
		ofn.lpstrCustomFilter = NULL;
		ofn.nMaxCustFilter = 0;
		ofn.nFilterIndex = 0;
		ofn.lpstrFile = strFilename;				//���ؽ��յ�ȫ·���ļ���
		ofn.nMaxFile = MAX_PATH;					//��������С
		ofn.lpstrFileTitle = strFileTitle;		    //���ؽ��յ��ļ���
		ofn.nMaxFileTitle = MAX_PATH;			    //��������С
		ofn.lpstrInitialDir = NULL;				    //��ʼĿ¼ΪĬ��
		ofn.lpstrTitle = NULL;						//�Ի������
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;    //��������
		ofn.nFileOffset = 0;						//�ļ������ַ����е���ʼλ��
		ofn.nFileExtension = 0;						//��չ�����ַ����е���ʼλ��
		ofn.lpstrDefExt = TEXT("bmp");			    //Ĭ����չ��
		ofn.lCustData = 0;
		ofn.lpfnHook = NULL;

		if (GetOpenFileName(&ofn))
		{
			//���ļ�ȫ·��������ȫ�ֱ�����
			strcpy(path, ofn.lpstrFile);
		}
		//����Bitmap����
		pb = loadBitmap(path);
		//����λͼ
		drawBitmap(pb, GetActiveWindow());
	}
	return;
	}
}

void Main() {
	InitGraphics();
	//�����ʼ��
	InitLayout();

	//ע��ص�����
	registerMouseEvent(MyMouseEventCallback);
	registerCharEvent(MyCharEventCallback);
	registerKeyboardEvent(MyKeyboardEventCallback);
	registerTimerEvent(MyTimerEventCallback);
	registerCommandEvent(MyCommandEventCallback);
	registerSizeEvent(MySizeEventCallback);
}

void InitLayout() {
	//���ô��ڱ���
	SetWindowTitle("ͼƬ�鿴��");

	//��������˵�
	HMENU hmenu = CreateMenu();	

	//�����ļ��˵��ĵ���ʽ�˵���
	HMENU file_pop_menu = CreatePopupMenu();
	AppendMenu(file_pop_menu, 0, FilePopItem1, "��");
	AppendMenu(file_pop_menu, 0, FilePopItem2, "����");
	AppendMenu(file_pop_menu, 0, FilePopItem3, "���Ϊ");
	AppendMenu(file_pop_menu, 0, FilePopItem4, "�ر�");
	//���ļ��˵��󶨵��˵�����
	AppendMenu(hmenu, MF_POPUP, file_pop_menu,"�ļ�");

	//�����༭�˵��ĵ���ʽ�˵���
	HMENU edit_pop_menu = CreatePopupMenu();
	AppendMenu(edit_pop_menu, 0, EditPopItem1, "��ת");
	AppendMenu(edit_pop_menu, 0, EditPopItem2, "����");
	AppendMenu(edit_pop_menu, 0, EditPopItem3, "�ü�");	
	//���༭�˵��󶨵��˵�����
	AppendMenu(hmenu, MF_POPUP, edit_pop_menu, "�༭");

	//���������˵��ĵ���ʽ�˵���
	HMENU help_pop_menu = CreatePopupMenu();
	AppendMenu(help_pop_menu, 0, HelpPopItem1, "�鿴����");
	AppendMenu(help_pop_menu, 0, HelpPopItem2, "����ͼƬ�鿴��");
	//�������˵��󶨵��˵�����
	AppendMenu(hmenu, MF_POPUP, help_pop_menu, "����");

	//������˵��󶨵���ǰ����
	SetMenu(GetActiveWindow(), hmenu);

	//����״̬��
	//CreateWindow(STATUSCLASSNAME, "״̬��", SBS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		//0, 0, 0, 0, GetActiveWindow(), StatusWindow, GethInstance(), NULL);

	//CreateStatusWindow(SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, "״̬��", GetActiveWindow(), StatusWindow);
}

void drawBitmap(Bitmap* pb, HWND hwnd) {

	HDC hdc = GetDC(hwnd);
	//�������豸DC����ݵ��ڴ�DC
	HDC hdcmem = CreateCompatibleDC(hdc);
	/*
	HBITMAP hBitmap = (HBITMAP)LoadImage(
		NULL,
		path,
		IMAGE_BITMAP,
		0, 0,
		LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		*/

	//���ڴ�DC�󶨵�λͼ����
	HBITMAP bitmap = CreateCompatibleBitmap(hdc, pb->info_header.bitmap_width, pb->info_header.bitmap_width);
	HBITMAP h1 = SelectObject(hdcmem, bitmap);
	
	//���ڴ�DC�ϻ���
	for (int i = 0;i < pb->info_header.bitmap_height;i++) {
		for (int j = 0;j < pb->info_header.bitmap_width;j++) {
			SetPixel(hdcmem, i , j , RGB(pb->color[i * pb->info_header.bitmap_width + j].r,
				pb->color[i * pb->info_header.bitmap_width + j].g,
				pb->color[i * pb->info_header.bitmap_width + j].b));
		}
	}

	//��ȡ��ǰ���ڴ�С
	RECT rect;
	GetWindowRect(GetActiveWindow(), &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	//���ڴ�DC�е����ݿ�����hdc
	BitBlt(hdc, width / 2 - pb->info_header.bitmap_width / 2, height / 2 - pb->info_header.bitmap_height / 2, 
		pb->info_header.bitmap_width, pb->info_header.bitmap_height, hdcmem, 0, 0, SRCCOPY);
}