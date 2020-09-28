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
	TwoDimensionalArray* frequencyDomain = bmp.FourierTransform();



	BMP* fourierdisplay = bmp.fourierTransformDisplay(*frequencyDomain);
	BMPDto out(*fourierdisplay);
	out.save("C:\\Users\\kim\\Desktop\\fourier.bmp");
	delete fourierdisplay;



	bmp.inversefourierTransform(*frequencyDomain);
	BMPDto out1(bmp);
	out1.save("C:\\Users\\kim\\Desktop\\changed.bmp");


	delete frequencyDomain;

}