#pragma once
#include "CNumber.h"

class TwoDimensionalArray {

public:
	int rowLength;
	int columnLength;
	Complex ** data;
	Complex* temp;
	TwoDimensionalArray(int rowNumber, int columnNumber) : rowLength(rowNumber), columnLength(columnNumber) {
		data = new CNumber*[rowNumber];
		for (int i = 0; i < rowNumber; i++) {
			data[i] = new Complex[columnLength];
		}
		temp = new Complex[rowLength];
	}

	~TwoDimensionalArray() {
		for (int i = 0; i < rowLength; i++) {
			delete[] data[i];
		}
		delete[] data;
		delete[] temp;
	}

	Complex* getRow(int row);

	Complex* getColumn(int column);

	void copyTemp2Column(int column);

	void print();
};
