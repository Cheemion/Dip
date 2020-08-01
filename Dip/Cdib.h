#pragma once
#include <windows.h>
#include <wingdi.h>

class CDib
{
public:
	BITMAPFILEHEADER bitmapFileHeader; //文件头 
	DWORD size; //除了bitmapFileHeader之外的大小
	BYTE* pDib; //除了bitmapFileHeader之外的数据头地址
	BITMAPINFOHEADER* m_pBitmapInfoHeader;//图片信息头地址
	RGBQUAD* m_pRGB;//颜色表首地址
	BITMAPINFO* m_pBitmapInfo; //和系统接口打交道需要用的实体类
	BOOL m_valid;
	BYTE* m_pData;//数据区
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
