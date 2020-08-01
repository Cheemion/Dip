#pragma once
#include <windows.h>
#include <wingdi.h>

class CDib
{
public:
	BITMAPFILEHEADER bitmapFileHeader; //�ļ�ͷ 
	DWORD size; //����bitmapFileHeader֮��Ĵ�С
	BYTE* pDib; //����bitmapFileHeader֮�������ͷ��ַ
	BITMAPINFOHEADER* m_pBitmapInfoHeader;//ͼƬ��Ϣͷ��ַ
	RGBQUAD* m_pRGB;//��ɫ���׵�ַ
	BITMAPINFO* m_pBitmapInfo; //��ϵͳ�ӿڴ򽻵���Ҫ�õ�ʵ����
	BOOL m_valid;
	BYTE* m_pData;//������
	UINT m_numberOfColors;
	
public:
	CDib();	
	~CDib();
	char m_fileName[256];
	char* GetFileName();
	BOOL IsValid();
	DWORD GetSize();
	UINT GetWidth();
	UINT GetHeight();
	UINT GetNumberOfColors();
	RGBQUAD* GetRGB();
	BYTE* GetData();
	BITMAPINFO* GetInfo();
	WORD PaletteSize(LPBYTE lpDIB);
	WORD DIBNumColors(LPBYTE lpDIB);
	void SaveFile(const char* filename);
	void LoadFile(const char* dibFileName);

};
