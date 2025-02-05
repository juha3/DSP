#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

float complex pll(float complex signal_in)
{
	static float phase_out = 0;
	static float frequency_out = 0;
	float complex signal_out;
	float phase_error;

	signal_out = cexpf(_Complex_I * phase_out);
	phase_error = cargf(signal_in * conjf(signal_out));
	phase_out += 0.05 * phase_error;
	frequency_out += 0.05 * 0.05 * 0.5 * phase_error;

	phase_out += frequency_out;
		
	static int i = 0;
	printf("%d %f %f %f %f\n", i++,
		crealf(signal_in), crealf(signal_out),
		phase_error, frequency_out);
	
	return signal_out;
}

int main(int argc, char *argv[])
{
	int ret;
	int i;
	float v1,v2;
	float complex signal_in;
	float complex signal_out;
	float phase_in = 3.0;

	for (i = 0; i < 1000; i++) {
		signal_in  = cexpf(_Complex_I * phase_in);
		signal_out = pll(signal_in);
		phase_in += -0.2;
	}	
	return 0;
}

