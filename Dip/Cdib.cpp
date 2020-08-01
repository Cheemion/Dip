#define _CRT_SECURE_NO_WARNINGS
#include "Cdib.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
CDib::CDib() {
	size = 0;
	m_valid = FALSE;
}

CDib::~CDib()
{

}

void CDib::LoadFile(const char* dibFileName) {

	FILE* fp = NULL;
	fp = fopen(dibFileName, "rb");
	if (fp == NULL)
		throw "something wrong with file";
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if (bitmapFileHeader.bfType != 0x4d42)
		throw "not bmp file";
	
	fseek(fp, 0, SEEK_END);
	DWORD allocateBytes = ftell(fp) - sizeof(BITMAPFILEHEADER);
	pDib = (BYTE*)malloc(allocateBytes);
	size = allocateBytes;

	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);
	fread(pDib, allocateBytes, 1, fp);
	fclose(fp);

	m_pBitmapInfo = (BITMAPINFO*)pDib;
	m_pBitmapInfoHeader = (BITMAPINFOHEADER*)pDib;
	m_pRGB = (RGBQUAD*)(pDib + m_pBitmapInfoHeader -> biSize);

	int numberOfColors = GetNumberOfColors();
	m_pData = (BYTE*)(m_pRGB + numberOfColors);
	if (m_pBitmapInfoHeader->biClrUsed == 0)
		m_pBitmapInfoHeader->biClrUsed = m_numberOfColors;

	if (m_pRGB == (RGBQUAD*)m_pData) // No color table
		m_pRGB = NULL;

	m_pBitmapInfoHeader->biSizeImage = GetSize();

	m_valid = TRUE;
}



void CDib::SaveFile(const char* dibFileName)
{
	FILE* fp;
	if ((fp = fopen(dibFileName, "wb")) == NULL)
		throw "something wrong with opening file";

	fwrite((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite((void*)pDib, size, 1, fp);
	fclose(fp);
}

UINT CDib::GetNumberOfColors()
{
	if (m_pBitmapInfoHeader->biClrUsed != 0)
		return m_pBitmapInfoHeader->biClrUsed;

	//24位 不带颜色表
	//其他位数，需要带颜色表，装入调色板中
	if (m_pBitmapInfoHeader->biBitCount == 24) {
		return 0;
	} else {
		UINT result = 1;
		for (WORD i = 0; i < m_pBitmapInfoHeader->biBitCount; i++) {
			result = result * 2;
		}
		return result;
	}
}

//得到像素个数而已
DWORD CDib::GetSize()
{
	if (m_pBitmapInfoHeader->biSizeImage != 0)
		return m_pBitmapInfoHeader->biSizeImage;
	else {
		DWORD height = (DWORD)GetHeight();
		DWORD width = (DWORD)GetWidth();
		return height * width;
	}
}

UINT CDib::GetWidth() {
	return (UINT)m_pBitmapInfoHeader->biWidth;
}

UINT CDib::GetHeight() {
	return (UINT)m_pBitmapInfoHeader->biHeight;
}
int main() {

	CDib cdib = CDib();
	cdib.LoadFile("C:\\Users\\kim\\Desktop\\test.bmp");
	cdib.SaveFile("C:\\Users\\kim\\Desktop\\copyTest.bmp");
	return 0;
}