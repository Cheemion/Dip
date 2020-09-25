#include "BMP.h"
#include "BMPDto.h"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "CNumber.h"

int main() {

	CNumber time[4] = {CNumber(1,0), CNumber(2,0) , CNumber(2,0) , CNumber(1,0)};
	CNumber	frequency[4];

	FFT(4, time, frequency);

	for (int i = 0; i < 4; i++) {
		std::cout << frequency[i] << std::endl;
	}

	inverseFFT(4, time, frequency);

	for (int i = 0; i < 4; i++) {
		std::cout << time[i] << std::endl;
	}

}