#include "BMP.h"
#include "BMPDto.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "CNumber.h"

int main() {

	BMPDto in("C:\\Users\\kim\\Desktop\\DIP_Material\\result.bmp");
	BMP bmp(in);
	bmp.FourierTransform();
	BMPDto out(bmp);
	out.save("C:\\Users\\kim\\Desktop\\tttt.bmp");

}