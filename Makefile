all: filtfilt pll iir_sos

pll: pll.o
	gcc -o pll pll.o -lm
pll.o: pll.c
	gcc -c pll.c

filtfilt: filtfilt.o
	gcc -o filtfilt filtfilt.o -lm
filtfilt.o: filtfilt.c
	gcc -c filtfilt.c
iir_sos: iir_sos.o
	gcc -o iir_sos iir_sos.o -lm
iir_sos.o: iir_sos.c 
	gcc -c iir_sos.c 

clean:
	rm -f *.o filtfilt pll iir_sos fir


