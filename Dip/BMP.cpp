#include "BMP.h"
#include "BMPDto.h"
#include <iostream>
#include <stdlib.h> // .h是c的内置  没有.h的是c++的内置
#include <new> 
#include <windows.h>
#include <wingdi.h>
#include <array>
#include <vector>
#include <algorithm>
#include <math.h>
#include "CNumber.h"
static constexpr auto WHITE = (BYTE)255;
static constexpr auto BLACK = (BYTE)0;

BMP::BMP(const BMPDto & bmpDto)
{
	new(this)BMP(bmpDto.m_ColorTableSize, bmpDto.m_DataSize, (BYTE*)bmpDto.m_ColorTable, bmpDto.m_Data, 
		         bmpDto.m_InfoHeader.biWidth, bmpDto.m_InfoHeader.biHeight, bmpDto.m_InfoHeader.biBitCount);
}
//this not like java, you have to manually set default value.
//要不然的话 就显示之前内存的值 很蛋疼
BMP::BMP(DWORD colorTalbeSize, DWORD dataSize, BYTE * colorTable, BYTE * data, DWORD width, DWORD height, WORD bitForPix)
	:m_Height(height), m_Width(width), m_BitForPix(bitForPix), m_DataBytes(dataSize), m_ColorTableBytes(colorTalbeSize)
{
	if (colorTalbeSize == 0)
		this->m_ColorTable = nullptr;
	else {
		this->m_ColorTable = (RGBQUAD*)malloc(colorTalbeSize);
		if (this->m_ColorTable == nullptr)
			throw "wrong allocate memory";
		memcpy(this->m_ColorTable, colorTable, colorTalbeSize);
	}
	this->m_Data = (BYTE*)malloc(dataSize);
	if (this->m_Data == nullptr)
		throw "wrong allocate memory";
	memcpy(this->m_Data, data, dataSize);
	if (is8Bit()) {
		m_ExtraBytesEachLine = (getWidth() + 3) / 4 * 4 - getWidth();
	} else {
		m_ExtraBytesEachLine = (getWidth() * 3 + 3) / 4 * 4 - getWidth() * 3;
	}
}

BMP::BMP(DWORD dataSize, DWORD width, DWORD height, WORD bitForPix)
	:m_DataBytes(dataSize), m_Width(width), m_Height(height), m_BitForPix(bitForPix)
{
	this->m_Data = (BYTE*)malloc(dataSize);
	if (this->m_Data == nullptr)
		throw "wrong allocate memory";
	if (is8Bit()) {
		m_ExtraBytesEachLine = (getWidth() + 3) / 4 * 4 - getWidth();
	} else {
		m_ExtraBytesEachLine = (getWidth() * 3 + 3) / 4 * 4 - getWidth() * 3;
	}
}

BMP::~BMP() {
	if (m_Data != nullptr)
		free(m_Data);
	if(m_ColorTable != nullptr)
		free(m_ColorTable);
}

DWORD BMP::getWidth() {
	return m_Width;
}

DWORD BMP::getHeight() {
	return m_Height;
}

WORD BMP::getBitForPix()
{
	return m_BitForPix;
}

RGBQUAD& BMP::getColorTable(BYTE index)
{
	return *(m_ColorTable + index);
}

BYTE & BMP::getPixByte(DWORD x, DWORD y)
{	 
	if (x > getWidth() || y > getHeight())
		throw "worng with cooridate";	// TODO: insert return statement here
	y = getHeight() - y - 1; //bmp图片上下颠倒 你看到的第一行的图片存在最后一行
	BYTE* byte = getData();
	byte = byte + y * (m_ExtraBytesEachLine + getWidth()) + x;
	return *byte;
}


BRG & BMP::getPixRGB(DWORD x, DWORD y)
{
	if (x > getWidth() || y > getHeight())
		throw "worng with cooridate";
	y = getHeight() - y - 1;
	BYTE* data = getData();
	data = data + y * m_ExtraBytesEachLine;
	BRG* result = (((BRG*)data) + x + y * getWidth());
	return *result;
}

bool BMP::is8Bit()
{
	return getBitForPix() == 8;
}


BYTE * BMP::getData()
{
	return m_Data;
}


BMP& BMP::leftRightMirror()
{	
	if (is8Bit()) { 
		processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
			return this->getPixByte(this->getWidth() - 1 - x, y);
		});
	} else {
		processForEachRGBPix([this](DWORD x, DWORD y) -> BRG {
			return this->getPixRGB(this->getWidth() - 1 - x, y);
		});
	}
	return *this;
}

BMP & BMP::medianFilter(DWORD N)// 不适合有许多点和线的图像 不考虑效率 不考虑elegant
{

	if (N % 2 != 1) throw "N should be a odd";
	if (is8Bit()) {
		BYTE* temp = new BYTE[N * N];
		processForEachBytePix([N, this, temp](DWORD x, DWORD y) -> BYTE {
			if (x < N / 2 || x > getWidth() - 1 - N / 2 || y < N / 2 || y > getHeight() - 1 - N / 2) {
				return getPixByte(x, y);
			} else {
				int count = 0;
				for (DWORD i = 0; i < N; i++) {
					for (DWORD j = 0; j < N; j++) {
						temp[count++] = getPixByte(x + i - N / 2, y + j - N / 2);
					}
				}
				std::sort(temp, temp + N * N);
				return temp[N * N / 2];
			}
		});
		delete[] temp;
	} else {
		BYTE* temp = new BYTE[N * N];
		processForEachRGBPix([N, this, temp](DWORD x, DWORD y) -> BRG {
			if (x < N / 2 || x > getWidth() - 1 - N / 2 || y < N / 2 || y > getHeight() - 1 - N / 2) {
				return getPixRGB(x, y);
			} else {

				BRG result;
				int count = 0;
				for (DWORD i = 0; i < N ; i++) {
					for (DWORD j = 0 ; j < N; j++) {
						temp[count++] = getPixRGB(x + i - N / 2 , y + j - N / 2).blue;
					}
				}
				std::sort(temp, temp + N * N);
				result.blue = temp[N * N / 2];

				count = 0;
				for (DWORD i = 0; i < N; i++) {
					for (DWORD j = 0; j < N; j++) {
						temp[count++] = getPixRGB(x + i - N / 2, y + j - N / 2).red;
					}
				}
				std::sort(temp, temp + N * N);
				result.red = temp[N * N / 2];

				count = 0;
				for (DWORD i = 0; i < N; i++) {
					for (DWORD j = 0; j < N; j++) {
						temp[count++] = getPixRGB(x + i - N / 2, y + j - N / 2).green;
					}
				}
				std::sort(temp, temp + N * N);
				result.green = temp[N * N / 2];
				return result;
			}
		});
		delete[] temp;
	}
	return *this;
}


void BMP::processForEachBytePix(const std::function<BYTE(DWORD x, WORD y)>& func)
{	
	BMP temp(m_DataBytes, getWidth(), getHeight(), getBitForPix());
	if (is8Bit()) {
		for (UINT y = 0; y < getHeight(); y++) {
			for (UINT x = 0; x < getWidth(); x++) {
				temp.getPixByte(x, y) = func(x, y);
			}
		}
		memcpy(getData(), temp.getData(), m_DataBytes);
	} else {

	}

}

void BMP::processForEachRGBPix(const std::function<BRG(DWORD x, WORD y)>& func)
{	
	BMP temp(m_DataBytes, getWidth(), getHeight(), getBitForPix());
	for (UINT y = 0; y < getHeight(); y++) {
		for (UINT x = 0; x < getWidth(); x++) {
			temp.getPixRGB(x, y) = func(x, y);
		}
	}
	memcpy(getData(), temp.getData(), m_DataBytes);
}

BMP& BMP::boundaryEnhancementByGradients()
{
	if (is8Bit()) {
		processForEachBytePix([this](DWORD x, DWORD y) -> BYTE{
			if (x == 0 || y == 0) return getPixByte(x, y);
			int xDifferential = getPixByte(x, y) - getPixByte(x - 1, y);
			int yDifferential = getPixByte(x, y) - getPixByte(x, y - 1);
			int gradient = (int)sqrt(xDifferential * xDifferential + yDifferential * yDifferential);
			if (gradient > 30) {
				return 255;
			} else {
				return 0;
			}
			//if (gradient + getPixByte(x, y) > 255)
			//	return 255;
			//else
			//	return gradient + getPixByte(x, y);
		});
	} else {
		processForEachRGBPix([this](DWORD x, DWORD y) -> BRG {
			if (x == 0 || y == 0) return getPixRGB(x, y);
			BRG result;
			{
				int xDifferential = getPixRGB(x, y).red - getPixRGB(x - 1, y).red;
				int yDifferential = getPixRGB(x, y).red - getPixRGB(x, y - 1).red;
				int gradient = (int)sqrt(xDifferential * xDifferential + yDifferential * yDifferential);
				if (gradient + getPixRGB(x, y).red > 255)
					result.red = 255;
				else
					result.red = gradient + getPixRGB(x, y).red;
			}
			{
				int xDifferential = getPixRGB(x, y).blue - getPixRGB(x - 1, y).blue;
				int yDifferential = getPixRGB(x, y).blue - getPixRGB(x, y - 1).blue;
				int gradient = (int)sqrt(xDifferential * xDifferential + yDifferential * yDifferential);
				if (gradient + getPixRGB(x, y).blue > 255)
					result.blue = 255;
				else
					result.blue = gradient + getPixRGB(x, y).blue;
			}
			{
				int xDifferential = getPixRGB(x, y).green - getPixRGB(x - 1, y).green;
				int yDifferential = getPixRGB(x, y).green - getPixRGB(x, y - 1).green;
				int gradient = (int)sqrt(xDifferential * xDifferential + yDifferential * yDifferential);
				if (gradient + getPixRGB(x, y).green > 255)
					result.green = 255;
				else
					result.green = gradient + getPixRGB(x, y).green;
			}

			return result;
		});
	}
	return *this;
}

BMP & BMP::boundaryEnhancementByKrisch()
{
	if (is8Bit()) {
		processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
			if (x == 0 || y == 0) return getPixByte(x, y);
			BYTE result = 0;
			int ntemplate[8][9] = { {5,5,5,-3,0,-3,-3,-3,-3},
									{-3,5,5,-3,0,5,-3,-3,-3},
									{-3,-3,5,-3,0,5,-3,-3,5},
									{-3,-3,-3,-3,0,5,-3,5,5},
									{-3,-3,-3,-3,0,-3,5,5,5},
									{-3,-3,-3,5,0,-3,5,5,-3},
									{5,-3,-3,5,0,-3,5,-3,-3},
									{5,5,-3,5,0,-3,-3,-3,-3}  };
			int difference = 0;
			for (int i = 0; i < 8; i++) {
				int temp = 0;
				temp = temp + getPixByte(x - 1, y - 1) * ntemplate[i][0];
				temp = temp + getPixByte(x    , y - 1) * ntemplate[i][1];
				temp = temp + getPixByte(x + 1, y - 1) * ntemplate[i][2];
				temp = temp + getPixByte(x - 1, y    ) * ntemplate[i][3];
				temp = temp + getPixByte(x    , y    ) * ntemplate[i][4];
				temp = temp + getPixByte(x + 1, y    ) * ntemplate[i][5];
				temp = temp + getPixByte(x - 1, y + 1) * ntemplate[i][6];
				temp = temp + getPixByte(x    , y + 1) * ntemplate[i][7];
				temp = temp + getPixByte(x + 1, y + 1) * ntemplate[i][8];
				temp = abs(temp);
				if (temp > difference)
					difference = temp;
			}
			if (difference + getPixByte(x, y) > 255)
				return 255;
			else
				return difference + getPixByte(x, y);
		});
	} else {
		//256先不实现
	};
	return *this;
}

 DWORD differenceABS(DWORD a, DWORD b) {
	if (a > b)
		return a - b;
	else
		return b - a;
}

 BYTE BMP::GetIterationThreshold()
 {

	 DWORD threshold = 0;

	 for (UINT y = 0; y < getHeight(); y++) {
		 for (UINT x = 0; x < getWidth(); x++) {
			 threshold = threshold + getPixByte(x, y);
		 }
	 }
	 threshold = threshold / (getHeight() * getWidth());

	 while (true) {
		 DWORD threshold1 = 0;
		 DWORD num1 = 0;
		 DWORD threshold2 = 0;
		 DWORD num2 = 0;
		 for (UINT y = 0; y < getHeight(); y++) {
			 for (UINT x = 0; x < getWidth(); x++) {
				 if (getPixByte(x, y) < threshold) {
					 threshold1 += getPixByte(x, y);
					 num1++;
				 }
				 else {
					 threshold2 += getPixByte(x, y);
					 num2++;
				 }
			 }
		 }
		 threshold1 = threshold1 / num1;
		 threshold2 = threshold2 / num2;
		 DWORD temp = (threshold1 + threshold2) / 2;
		 if (differenceABS(temp, threshold) < 3)
			 break;

		 threshold = temp;
		 std::cout << "***************** threshold is " << threshold << "********************" << std::endl;
		 return (BYTE)threshold;
	 }
 }

BMP & BMP::imageBinaryzation(BYTE threshold)
{

	processForEachBytePix([this, threshold](DWORD x, DWORD y) -> BYTE {
		if (getPixByte(x, y) > threshold)
			return 255;
		else
			return 0;
	});

	return *this;
}



BMP & BMP::edgeExtraction()
{

	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (getPixByte(x, y) == 255)
			return getPixByte(x, y);

		if (getPixByte(x, y) == getPixByte(x - 1, y - 1)
			&& getPixByte(x, y) == getPixByte(x - 1, y)
			&& getPixByte(x, y) == getPixByte(x - 1, y+1)
			&& getPixByte(x, y) == getPixByte(x, y-1)
			&& getPixByte(x, y) == getPixByte(x, y+1)
			&& getPixByte(x, y) == getPixByte(x + 1, y-1)
			&& getPixByte(x, y) == getPixByte(x + 1, y)
			&& getPixByte(x, y) == getPixByte(x + 1, y+1)) {
			return 255;
		}
		return getPixByte(x, y);
	});

	return *this;
}

static void renderNearColor(BMP& bmp, UINT x, UINT y ,BYTE color, int& area) {
	if (x == 0 || y == 0 || x == bmp.getWidth() - 1 || y == bmp.getHeight() - 1)
		return;
	if (bmp.getPixByte(x, y) == 255)
		return;
	if (bmp.getPixByte(x, y) == color)
		return;

	area++;
	bmp.getPixByte(x, y) = color;

	renderNearColor(bmp, x    , y + 1, color, area);
	renderNearColor(bmp, x + 1, y + 1, color, area);
	renderNearColor(bmp, x + 1, y    , color, area);
	renderNearColor(bmp, x - 1, y + 1, color, area);
	renderNearColor(bmp, x - 1, y    , color, area);

	//上面的像素可以不跑 因为检测是从上往下检测的
	//renderNearColor(bmp, x    , y - 1, color);
	//renderNearColor(bmp, x + 1, y - 1, color);
	//renderNearColor(bmp, x - 1, y - 1, color);

}

BMP & BMP::objectSpot()
{	
	BYTE count = 0;
	BMP temp(m_DataBytes, getWidth(), getHeight(), getBitForPix());
	memcpy(temp.getData(), getData(), m_DataBytes);

	if (is8Bit()) {
		for (UINT y = 0; y < getHeight(); y++) {
			for (UINT x = 0; x < getWidth(); x++) {

				if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
					continue;
				
				if (temp.getPixByte(x, y) != 0) //不是黑就下一个
					continue;

				int area = 0;
				renderNearColor(temp, x, y, ++count, area);
				std::cout << "count:" << (int)count << " area" << area << std::endl;
			}
			memcpy(getData(), temp.getData(), m_DataBytes);
		}
	} else { //先不管24位

	}

	std::cout << "this number of objects is " << (int)count << std::endl;
	return *this;
}

static bool isWhite(BYTE a) {
	return a == 255;
}

static bool isBlack(BYTE a) {
	return a == 0;
}
BMP & BMP::verticalDilation()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isWhite(getPixByte(x, y)) && (isBlack(getPixByteUp(x, y)) || isBlack(getPixByteDown(x, y))))
			return BLACK;

		return getPixByte(x, y);
	});
	return *this;
}

BMP & BMP::horizontalDilation()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isWhite(getPixByte(x, y)) && (isBlack(getPixByteLeft(x, y)) || isBlack(getPixByteRight(x, y))))
			return BLACK;
	
		return getPixByte(x, y);

	});
	return *this;
}

BMP & BMP::crossDilation()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isWhite(getPixByte(x, y)) && (isBlack(getPixByteLeft(x, y)) || isBlack(getPixByteRight(x, y)) || isBlack(getPixByteUp(x, y)) || isBlack(getPixByteDown(x, y))))
			return BLACK;
	
		return getPixByte(x, y);
	});
	return *this;
}



BMP & BMP::verticalErosion()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isBlack(getPixByte(x, y)) && (!isBlack(getPixByteUp(x, y)) || !isBlack(getPixByteDown(x, y))))
			return WHITE;

		return getPixByte(x, y);
	});
	return *this;
}

BMP & BMP::horizontalErosion()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isBlack(getPixByte(x, y)) && (!isBlack(getPixByteLeft(x, y)) || !isBlack(getPixByteRight(x, y)) ))
			return WHITE;

		return getPixByte(x, y);
	});
	return *this;
}

BMP & BMP::crossErosion()
{
	processForEachBytePix([this](DWORD x, DWORD y) -> BYTE {
		if (x == 0 || y == 0 || x == getWidth() - 1 || y == getHeight() - 1)
			return getPixByte(x, y);

		if (isBlack(getPixByte(x, y)) && (!isBlack(getPixByteLeft(x, y)) || !isBlack(getPixByteRight(x, y)) || !isBlack(getPixByteUp(x, y)) || !isBlack(getPixByteDown(x, y))))
			return WHITE;

		return getPixByte(x, y);
	});
	return *this;
}


//not finished
BMP & BMP::imageThining()
{
	return *this;
}

BMP & BMP::FT()
{
	CNumber* cnumbers = new CNumber[getHeight() * getWidth()];
	double* ft = new double[getHeight() * getWidth()];
	BMP temp(m_DataBytes, getWidth(), getHeight(), getBitForPix());

	for (UINT y = 0; y < getHeight(); y++) {
		for (UINT x = 0; x < getWidth(); x++) {
			double test = (double)getPixByte(x, y);
			ft[y * getWidth() + x] = (double)getPixByte(x, y);
		}
	}

//	TwoDimensionDFT(getHeight(), getWidth(), ft, cnumbers);

	for (UINT y = 0; y < getHeight(); y++) {
		for (UINT x = 0; x < getWidth(); x++) {
			CNumber c = cnumbers[y * getWidth() + x];
			double t = cnumbers[y * getWidth() + x].im * cnumbers[y * getWidth() + x].im + cnumbers[y * getWidth() + x].re * cnumbers[y * getWidth() + x].re;
			t = sqrt(t);
			if (t > 254.99)
				temp.getPixByte(x, y) = 255;
			else
				temp.getPixByte(x, y) = round(t);
		}
	}
	
	memcpy(getData(), temp.getData(), m_DataBytes);


	delete[] cnumbers;
	delete[] ft;
	return *this;
}


BMPDto * BMP::convertToDto() {
	return new BMPDto(m_Width, m_Height, m_BitForPix, m_ColorTableBytes, m_DataBytes, m_ColorTable, m_Data);
}
