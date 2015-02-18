CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -c `pkg-config --cflags gtk+-2.0` -D_GNU_SOURCE -O2
LIBS = `pkg-config --libs gtk+-2.0`

ffxcast:		ffmpeg.o main.o settings.o ui.o xwininfo.o
				$(CC) -s -o ffxcast ffmpeg.o main.o settings.o ui.o xwininfo.o $(LIBS)

ffmpeg.o:		ffmpeg.c ffmpeg.h settings.h ui.h
				$(CC) $(CFLAGS) ffmpeg.c

main.o:			main.c ui.h settings.h
				$(CC) $(CFLAGS) main.c

settings.o:		settings.c settings.h ui.h
				$(CC) $(CFLAGS) settings.c

ui.o:			ui.c ui.h settings.h ffmpeg.h xwininfo.h
				$(CC) $(CFLAGS) ui.c

xwininfo.o:		xwininfo.c ui.h settings.h xwininfo.h
				$(CC) $(CFLAGS) xwininfo.c

clean:
				rm -f *.o
