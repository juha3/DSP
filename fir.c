#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



/*

octave:

freqz(fir1(3,0.5))

=> [0.016836   0.483164   0.483164   0.016836]

fir1(8, 0.5)

[-0.000077721  -0.022665117   0.000524615   0.273997637   0.496441171   0.273997637   0.000524615  -0.022665117  -0.000077721]

decimation

	- LPF
	- pick every Nth sample (downsample)

interpolation
	- upsample (zero stuffing)
	- LPF (optimize zero-ops)
	- Corner frequency =  Fs / (2 * upsampling factor), ie. Fs / (2 * 8) = Fs / 16 => fir1(NN, 1 / 16)
	- Amplify ouput by K, as it is attenuated, for example: K = 2, gain = 0.5
*/

struct fir {
	float b[9];
	float x[9];
};

/* 
*/

float fir_filter(struct fir *fir, float x)
{
	int i;
	float y = 0;

	fir->x[0] = x;
	for (i = 0; i < 9; i++)
		y = y + fir->b[i] * fir->x[i];

	for (i = 1; i < 9; i++)
		fir->x[8 - i + 1] = fir->x[8 - i]; 
	
	return y;
}


int main()
{
	struct fir s = {
		.b = {-0.000077721, -0.022665117, 0.000524615, 0.273997637, 0.496441171, 0.273997637, 0.000524615, -0.022665117, -0.000077721},
		.x = {0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	int i;
	float v, v1;
	

	for (i = 0; i < 1000; i++) {
		v = sin(2 * M_PI * 10 * i / 1000); /* sampling rate 1000 Hz, 50 Hz sine wave */
		v += sin(2 * M_PI * 450 * i / 1000.0);
		v1 = fir_filter(&s, v);
		printf("%d %f %f\n", i, v, v1); /* sample number, input signal, filtered signal */
			
	
	}


	return 0;
}

