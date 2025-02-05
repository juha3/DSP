#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



/*

octavessa:

[b,a] = butter(4, 0.5)
[ss, gn] = tf2sos(b,a)

         +-                     -+
         | b01 b11 b21 1 a11 a21 |
         | b02 b12 b22 1 a12 a22 |
    ss = |  .   .   .  .  .   .  |
         |  .   .   .  .  .   .  |
         | b0L b1L b2L 1 a1L a2L |
         +-                     -+
    
     H = gn * tf([b01 b11 b21], [1 a11 a21], Ts) * ... * ...
    
	joka rivi: [B0 B1 B2 A0 A1 A2], jossa A0 = 1.000

*/

struct sos {
	float a[2];
	float b[3];
	float s1[2];
	float s2[2];
};

/* 
	fixed point => non-transposed form
	floating point => transposed form

	IIR second order section implemented in direct form 2. 

	https://en.wikipedia.org/wiki/Digital_biquad_filter#Direct_form_2

	y[n] = b0 * w[n] + b1 * w[n - 1] + b2 * w[n - 2]
	w[n] = x[n] -1 * a1 * w[n - 1] + -1 * w[n - 2]i

	Direct form 2 transposed:

	y[n] = b0 * x[n] + s1[n - 1]
	s1[n] = s2[n - 1] + b1 * x[n] - a1 * y[n]
	s2[n] = b2 * x[n] - a2 * y[n]

*/

/* transposed form */

float iir_sos(struct sos *sos, float x)
{
	float y;

	sos->s1[0] = sos->s2[1] + sos->b[1] * x + -1 * sos->a[1] * y;
	sos->s2[0] = sos->b[2] * x + -1 * sos->a[2] * y;
	y = sos->b[0] * x + sos->s1[1];

	sos->s1[1] = sos->s1[0];
	sos->s2[1] = sos->s2[0];
	
	return y;
}

/* 1st section */
#define A1 -1.89012
#define A2  0.98739
#define B0  1.00000
#define B1 -1.90212
#define B2  1.00000

/* gain */
#define GN 0.9936

int main()
{
	struct sos s;
	int i;
	float v, v1;

	memset(&s, 0, sizeof(s));
	s.a[1] = A1;
	s.a[2] = A2;
	s.b[0] = B0;
	s.b[1] = B1;
	s.b[2] = B2;

	for (i = 0; i < 1000; i++) {
		v = sin(2 * M_PI * 50 * i / 1000); /* sampling rate 1000 Hz, 50 Hz sine wave */
		v1 = GN * iir_sos(&s, v);
		printf("%d %f %f\n", i, v, v1); /* sample number, input signal, filtered signal */
			
	
	}


	return 0;
}

