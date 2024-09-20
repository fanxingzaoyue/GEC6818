CC = arm-linux-g++

CPPFLAGS += -I ./inc
CPPFLAGS += -I ./inc/libxml2
LDFLAGS  += -L ./lib

LDFLAGS += -lxml2
LDFLAGS += -lz
LDFLAGS += -lpthread

main: common.cpp video.cpp lcd.cpp main.cpp  touch.cpp bmp.cpp photo.cpp music.cpp video.cpp v4l2.cpp v4use.cpp
	$(CC) $^ -std=c++11 -o   $@ $(CPPFLAGS) $(LDFLAGS) -Wall

clean:
	rm main -rf
