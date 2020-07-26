#include "windows.h"
#include <wingdi.h>
#include <string>
class CDib
{
public:
	RGBQUAD* m_pRGB;
	BYTE* m_pData;
	UINT m_numberOfColors;
	BOOL m_valid;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER* m_pBitmapInfoHeader;
	BITMAPINFO* m_pBitmapInfo;
	BYTE* pDib;
	DWORD size;
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
	void SaveFile(const std::string filename);
	void LoadFile(const std::string dibFileName);

};
