#pragma once
#include <windows.h>
#include <wingdi.h>
#include "BRG.h"
class BMP;
class BMPDto {
	friend class BMP;
private:
	const char* m_FileDir;
	BITMAPFILEHEADER m_FileHeader;
	BITMAPINFOHEADER m_InfoHeader;
	RGBQUAD* m_ColorTable;
	BYTE* m_Data;
	DWORD m_ColorTableSize;
	DWORD m_DataSize;
public:
	BMPDto(const char* fileDir);
	~BMPDto();
	BMP* convert2BMP();
	void save(const char* destination);
	BMPDto(DWORD width, DWORD height, WORD bitForPix, DWORD colorTableSize, DWORD dataSize, void * colorTable, void * data);
	BMPDto(BMP& bmp);
};