PREFIX = /home/alex/Android/android-ndk-r26-beta1/toolchains/llvm/prebuilt/linux-x86_64/bin
CC = $(PREFIX)/armv7a-linux-androideabi28-clang

SOURCES = $(wildcard *.c)
TARGETS = $(SOURCES:.c=)
OBJECTS = $(SOURCES:.c=.o)
ANDROID_CFLAGS = -static-libgcc  -fPIE -pie

.PHONY: all clean

all: $(TARGETS)

$(TARGETS): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGETS)

%.o: %.c
	$(CC) $(ANDROID_CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS) $(TARGETS)

