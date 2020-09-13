#include "BMP.h"
#include "BMPDto.h"
#include <iostream>
#include <string>
int main() {

	BMPDto bmpDto = BMPDto("c:\\users\\kim\\desktop\\DIP_Material\\ª“∂»Õº.bmp");

	BMP bmp(bmpDto);

	bmp.edgeExtraction();

	BMPDto result(bmp);

	result.save("c:\\users\\kim\\desktop\\result11.bmp");

}