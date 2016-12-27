CC = g++

main : main.o converter.o yuv.o
	$(CC) -o main main.o converter.o yuv.o

main.o : main.cpp rgb.h yuv.h converter.h
	$(CC) -c main.cpp

converter.o : converter.cpp converter.h rgb.h yuv.h
	$(CC) -c converter.cpp

yuv.o : yuv.cpp yuv.h
	$(CC) -c yuv.cpp

clean :
	rm main *.o