#pragma once
#include <windows.h>
#include <wingdi.h>
#include <functional>
class BRG;
class BMPDto;

class BMP {
	friend class BMPDto;
protected:
	DWORD m_Width;
	DWORD m_Height;
	WORD m_BitForPix;
	RGBQUAD* m_ColorTable; // we only thing about 8bit bmp for simpilicity
	DWORD m_ColorTableBytes;
	BYTE* m_Data;
	DWORD m_DataBytes;
	DWORD m_ExtraBytesEachLine;
public:
	BMP() {};
	BMP(const BMPDto& bmpDto);
	BMP(DWORD colorTableSize, DWORD dataSize, BYTE* colorTable, BYTE* data, DWORD width, DWORD height, WORD bitForPix);
	BMP(DWORD dataSize, DWORD width, DWORD height, WORD bitForPix);
	~BMP();
	DWORD getWidth();

	DWORD getHeight();
	
	WORD getBitForPix();

	RGBQUAD& getColorTable(BYTE index);

	BYTE& getPixByte(DWORD x, DWORD y);

	BYTE& getPixByteUp(DWORD x, DWORD y) {
		return getPixByte(x , y - 1);
	}

	BYTE& getPixByteDown(DWORD x, DWORD y) {
		return getPixByte(x , y + 1);
	}

	BYTE& getPixByteRight(DWORD x, DWORD y) {
		return getPixByte(x + 1, y);
	}
	BYTE& getPixByteLeft(DWORD x, DWORD y) {
		return getPixByte(x - 1, y);
	}
	BYTE& getPixByteTopLeft(DWORD x, DWORD y) {
		return getPixByte(x - 1, y - 1);
	}
	BYTE& getPixByteTopRight(DWORD x, DWORD y) {
		return getPixByte(x + 1, y - 1);
	}
	BYTE& getPixByteBottomLeft(DWORD x, DWORD y) {
		return getPixByte(x - 1, y + 1);
	}
	BYTE& getPixByteBottomRight(DWORD x, DWORD y) {
		return getPixByte(x + 1, y + 1);
	}

	BRG& getPixRGB(DWORD x, DWORD y);
	bool is8Bit();
	BMPDto* convertToDto();
	BYTE* getData(); 
	BMP& leftRightMirror();//左右镜像
	BMP & medianFilter(DWORD N);//N * N 中值滤波
	void processForEachBytePix(const std::function<BYTE(DWORD x, WORD y)>& func);
	void processForEachRGBPix(const std::function<BRG(DWORD x, WORD y)>& func);
	BMP& boundaryEnhancementByGradients(); //根据梯度
	BMP& boundaryEnhancementByKrisch(); //根据梯度
	BYTE GetIterationThreshold();//二值化
	BMP& imageBinaryzation(BYTE threshold);//二值化
	BMP& edgeExtraction();//对于背景是白 轮廓是黑的图像的轮廓的提取 掏空法
	BMP& objectSpot();

	BMP& verticalDilation();
	BMP& horizontalDilation();
	BMP& crossDilation();

	BMP& verticalErosion();
	BMP& horizontalErosion();
	BMP& crossErosion();

	BMP& imageThining();
	//virtual void processForEachPix(const std::function<BYTE(DWORD x, WORD y)>& func) = 0;
};