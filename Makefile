CC = gcc
CFLAGS = -Wall -Wextra
#CFLAGS = -W
TARGET = sp2
SRC = sp.c splib.c

#file output for sp2
#SRC += outputs/output-file.c

#alsa output for sp2 (RECOMMENDED)
SRC += outputs/alsa.c
LIBS += -lasound

#wav decoder
SRC += codecs/wav.c

#aac decoder
SRC += codecs/aac.c
LIBS += -lfaad

#mp3 decoder
SRC += codecs/mp3.c
LIBS += -lmp3lame

#ogg decoder
SRC += codecs/ogg.c
LIBS += -lvorbisfile

#spc decoder
APUCORE = deps/snes_spc-0.9.0/snes_spc
CFLAGS += -fno-exceptions
#inline -I
SRC += -I$(APUCORE) $(APUCORE)/*.cpp codecs/spc.c

#txt file decoder
#SRC += codecs/tts.c
#LIBS += -lespeak

#echo filter
SRC += filters/echo.c

DEBUG_FLAGS = -g3
RELEASE_FLAGS = -O3

all: $(TARGET)

$(TARGET):
	$(CC) $(DEBUG_FLAGS) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

release:
	$(CC) $(RELEASE_FLAGS) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm $(TARGET)
