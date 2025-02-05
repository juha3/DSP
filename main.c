#include <stdint.h>
#include <stdio.h>
#include <math.h>




/*

octavessa:

fs =  1000 // sampling rate
fs2 = fs/2
f = 50  // notch frequency
bw = 2 // notch bandwidth

[b,a] = pei_tseng_notch (f / fs2, bw/fs2)


(matlabissa vastaava komento on iir_notch.)

b =

   0.99369  -1.89012   0.99369

a =

   1.00000  -1.89012   0.98739

=> y[n] = 1.89012 * y[n - 1] - 0.98739 * y[n - 2] + 0.99369 * x[n] - 1.89012 * x[n - 1] + 0.99369 * x[n - 2]


*/

#define A1 -1.89012
#define A2  0.98739
#define B0  0.99369
#define B1 -1.89012
#define B2  0.99369


float iir_notch(float input)
{
	static float y[3] = {0};
	static float x[3] = {0};

	x[0] = input;
	y[0] = -1 * A1 * y[1] + -1 * A2 * y[2] + B0 * x[0] + B1 * x[1] + B2 * x[2];
	y[2] = y[1];
	y[1] = y[0];
	x[2] = x[1];
	x[1] = x[0];
	
	return y[0];
}


int main()
{

	int i;
	float v, v1;

	for (i = 0; i < 1000; i++) {
		v = sin(2 * M_PI * 55 * i / 1000); /* sampling rate 1000 Hz, 50 Hz sine wave */
		v1 = iir_notch(v);
		printf("%d %f %f\n", i, v, v1); /* sample number, input signal, filtered signal */
			
	
	}


	return 0;
}

