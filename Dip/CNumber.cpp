#include "CNumber.h"
#include <math.h>
#include <iostream>
static double PI = acos(-1);
static void InverseFFTHelper(Complex * a, int lim);
void InverseDFT(int N, CNumber * time, CNumber * frequency)
{
	for (int k = 0; k < N; k++) {
		time[k].re = 0;
		time[k].im = 0;
		for (int n = 0; n < N; n++) {
			time[k] = time[k] + frequency[n] * CNumber((1.0) * 2 * PI / N * k * n);
		}
		time[k] = time[k] * CNumber(1.0 / N, 0);
	}
}

//ÆÕÍ¨¸µÀïÒ¶±ä»»
void DFT(int N, CNumber* time, CNumber* frequency) {
	for (int k = 0; k < N; k++) {
		frequency[k].re = 0.0;
		frequency[k].im = 0.0;
		for (int n = 0; n < N; n++) {
			frequency[k] = frequency[k] + time[n] * CNumber((-1.0) * 2 * PI / N * k * n);
		}
	}
}



Complex fftHelper(Complex * a, int N, int k) {
	if (N == 1) return a[0];

	Complex* a0 = new Complex[N / 2];
	Complex* a1 = new Complex[N / 2];

	for (int i = 0; i < N / 2; i++) {
		a0[i] = a[i * 2];
		a1[i] = a[i * 2 + 1];
	}

	Complex WNk = Complex(cos(-2.0 * PI * k / N), sin(-2.0 * PI * k / N));
	CNumber result = fftHelper(a0, N / 2, k) + WNk * fftHelper(a1, N / 2, k);

	delete[] a0;
	delete[] a1;
	return result;
}

void FFT(int N, CNumber* time, CNumber* frequency) {
	for (int k = 0; k < N; k++) {
		frequency[k] = fftHelper(time, N, k);
	}
}


static void FFThelper(Complex * a, int lim) {

	if (lim == 1) return;

	Complex* a0 = new Complex[lim / 2];
	Complex* a1 = new Complex[lim / 2];

	for (int i = 0; i < lim / 2; i++) {
		a0[i] = a[i * 2];
		a1[i] = a[i * 2 + 1];
	}

	FFT(a0, lim / 2);
	FFT(a1, lim / 2);
	Complex wn = Complex(cos(-2.0 * PI / lim), sin(-2.0 * PI / lim));
	Complex w = Complex(1.0, 0.0);

	for (int k = 0; k < lim / 2; k++) {
		a[k] = a0[k] + w * a1[k];
		a[k + lim / 2] = a0[k] - w * a1[k];
		w = w * wn;
	}

	delete[] a0;
	delete[] a1;

}


void FFT(Complex * a, int lim) {
	if (lim == 1) return;

	Complex* a0 = new Complex[lim >> 1];
	Complex* a1 = new Complex[lim >> 1];

	for (int i = 0; i < lim; i = i + 2) {
		a0[i >> 1] = a[i];
		a1[i >> 1] = a[i + 1];
	}

	FFT(a0, lim >> 1);
	FFT(a1, lim >> 1);
	Complex wn = Complex(cos(-2.0 * PI / lim), sin(-2.0 * PI / lim));
	Complex w = Complex(1.0, 0.0);

	for (int k = 0; k < (lim >> 1); k++) {
		a[k] = a0[k] + w * a1[k];
		a[k + (lim >> 1)] = a0[k] - w * a1[k];
		w = w * wn;
	}

	delete[] a0;
	delete[] a1;
}


void inverseFFT(int N, CNumber * time, CNumber * frequency) {
	int expandNumber = 1 << (int)(log(N) / log(2));
	CNumber* expandFrequency = new CNumber[expandNumber];
	for (int i = 0; i < expandNumber; i++) {
		if (i < N)
			expandFrequency[i] = frequency[i];
		else
			expandFrequency[i] = CNumber(0, 0);
	}

	inverseFFT(expandFrequency, N);

	for (int i = 0; i < N; i++) {
		time[i] = expandFrequency[i];
	}
}


void inverseFFT(Complex * a, int lim)
{	

	InverseFFTHelper(a, lim);
	Complex N(1.0 / lim, 0.0);
	for (int i = 0; i < lim; i++) {
		a[i] = N * a[i];
	}

}


static void InverseFFTHelper(Complex * a, int lim) {
	if (lim == 1) return;

	Complex* a0 = new Complex[lim >> 1];
	Complex* a1 = new Complex[lim >> 1];

	for (int i = 0; i < lim; i = i + 2) {
		a0[i >> 1] = a[i];
		a1[i >> 1] = a[i + 1];
	}

	InverseFFTHelper(a0, lim >> 1);
	InverseFFTHelper(a1, lim >> 1);
	Complex wn = Complex(cos(2.0 * PI / lim), sin(2.0 * PI / lim));
	Complex w = Complex(1.0, 0.0);

	for (int k = 0; k < (lim >> 1); k++) {
		a[k] = a0[k] + w * a1[k];
		a[k + (lim >> 1)] = a0[k] - w * a1[k];
		w = w * wn;
	}

	delete[] a0;
	delete[] a1;
}