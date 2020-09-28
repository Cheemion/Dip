#include "TwoDimensionalArray.h"


Complex* TwoDimensionalArray::getRow(int row) {
	return data[row];
}

Complex* TwoDimensionalArray::getColumn(int column) {
	for (int i = 0; i < rowLength; i++) {
		temp[i] = data[i][column];
	}
	return temp;
}

void TwoDimensionalArray::copyTemp2Column(int column) {
	for (int i = 0; i < rowLength; i++) {
		data[i][column] = temp[i];
	}
}

void TwoDimensionalArray::print() {
	int i = 0;
	for (int j = 0; j < columnLength; j++) {
		std::cout << data[i][j] << " ";
	}
}