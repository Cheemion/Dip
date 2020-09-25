#pragma once
#include<math.h>
#include<iostream>
#include <windows.h>
#include <wingdi.h>

class CNumber {
public:
	CNumber(double real, double imaginary) : re(real), im(imaginary) {}
	CNumber(double eToJPower) : re(cos(eToJPower)), im(sin(eToJPower)) {}
	CNumber(): re(0.0), im(0.0) {};
	~CNumber() {}

	friend std::ostream & operator<<(std::ostream & os, const CNumber & c) {
		os << "(" << c.re << ")" << " + " << "(" << c.im << ")" << "i"; //以"a+bi"的形式输出
		return os;
	}

	CNumber operator+(const CNumber& a) {
		return CNumber(this->re + a.re, this->im + a.im);
	}

	CNumber operator-(const CNumber& a) {
		return CNumber(this->re - a.re, this->im - a.im);
	}

	CNumber operator*(const CNumber& a) {
		CNumber temp;
		temp.re = this->re * a.re - this->im * a.im;
		temp.im = this->im * a.re + this->re * a.im;
		return temp;
	}

	CNumber operator/(const CNumber& a) {
		CNumber temp;
		temp.re = (this->re * a.re + this->im * a.im) / (a.re * a.re + a.im * a.im);
		temp.re = (this->im * a.re - this->re * a.im) / (a.re * a.re + a.im * a.im);
		return temp;
	}

public:
	double re;
	double im;
};

typedef CNumber Complex;

//1D 傅里叶逆变换
void InverseDFT(int N, CNumber* time, CNumber* frequency);

//1D 离散傅里叶变换
void DFT(int N, CNumber* time, CNumber* frequency);

//1D 快速傅里叶变换
void FFT(int N, CNumber* time, CNumber* frequency);

//1D 快速傅里叶逆变换
void inverseFFT(int N, CNumber* time, CNumber* frequency);

void FFT(Complex* a, int lim);

//1D 快速傅里叶逆变换
void inverseFFT(Complex* a, int lim);