#define _CRT_SECURE_NO_WARNINGS
#include "BMPDto.h"
#include <stdio.h>
#include "BMP.h"
#include <new>
BMPDto::BMPDto(const char * fileDir)
{
	this->m_FileDir = fileDir;
	FILE* fp = fopen(fileDir, "rb");
	if (!fp)
		throw "fileDir doesn't exist";
	fread(&m_FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&m_InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if (m_FileHeader.bfType != 0x4d42)
		throw "not bmp file";
	//只考虑8位图像或者24位真彩色图像
	if (m_InfoHeader.biBitCount == 8) {
		m_ColorTableSize = 256 * sizeof(RGBQUAD);
		m_ColorTable = (RGBQUAD*)malloc(m_ColorTableSize);
		if (!m_ColorTable)
			throw "memory error";
		fread(m_ColorTable, sizeof(RGBQUAD), 256, fp);
	} else if (m_InfoHeader.biBitCount == 24) {
		m_ColorTableSize = 0;
		m_ColorTable = nullptr;
	} else {
		throw "not 8bit or 24bit file";
	}
	m_DataSize = m_FileHeader.bfSize - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) - m_ColorTableSize;
	m_Data = (BYTE*)malloc(m_DataSize);
	if (!m_Data) throw "memory error";
	fread(m_Data, m_DataSize, 1, fp);
	fclose(fp);
}

BMPDto::~BMPDto()
{
	free(m_ColorTable);
	free(m_Data);
}

BMP* BMPDto::convert2BMP()
{
	return new BMP(m_ColorTableSize, m_DataSize, (BYTE*)m_ColorTable, m_Data, m_InfoHeader.biWidth, m_InfoHeader.biHeight, m_InfoHeader.biBitCount);
}

void BMPDto::save(const char* destination)
{
	FILE* fp = fopen(destination, "wb");
	if (!fp)
		throw "something wrong with destination dir";

	fwrite((void*)&m_FileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite((void*)&m_InfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	if(m_ColorTableSize != 0)
		fwrite(m_ColorTable, m_ColorTableSize, 1, fp);
	fwrite(m_Data, m_DataSize, 1, fp);
	fclose(fp);
}

BMPDto::BMPDto(DWORD width, DWORD height, WORD bitForPix, DWORD colorTableSize, DWORD dataSize, void * colorTable, void * data)
	: m_FileDir(nullptr), m_ColorTableSize(colorTableSize), m_DataSize(dataSize)
{	
	// fileHeader *********************
	this->m_FileHeader.bfType = 0x4d42;
	this->m_FileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize + dataSize;
	this->m_FileHeader.bfReserved1 = 0;
	this->m_FileHeader.bfReserved2 = 0;
	this->m_FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + colorTableSize;
	// infoHeader *******************8*
	this->m_InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	this->m_InfoHeader.biWidth = width;
	this->m_InfoHeader.biHeight = height;
	this->m_InfoHeader.biPlanes = 1;
	this->m_InfoHeader.biBitCount = bitForPix;
	this->m_InfoHeader.biCompression = 0;
	this->m_InfoHeader.biSizeImage = width * height * bitForPix / 8;
	this->m_InfoHeader.biXPelsPerMeter = 0;
	this->m_InfoHeader.biYPelsPerMeter = 0;
	this->m_InfoHeader.biClrUsed = 0;
	this->m_InfoHeader.biClrImportant = 0;
	//**********************************
	if (colorTableSize != 0) {
		m_ColorTable = (RGBQUAD*)malloc(colorTableSize);
		if (!m_ColorTable)
			throw "memory error";
		memcpy(m_ColorTable, colorTable, colorTableSize);
	} else {
		m_ColorTable = nullptr;
	}

	m_Data = (BYTE*)malloc(dataSize);
	if (!m_Data)
		throw "memory error";
	memcpy(m_Data, data, dataSize);
}

BMPDto::BMPDto(BMP & bmp)
{
	new(this)BMPDto(bmp.m_Width, bmp.m_Height, bmp.m_BitForPix, bmp.m_ColorTableBytes, bmp.m_DataBytes, (void*)bmp.m_ColorTable, (void*)bmp.m_Data);
}
