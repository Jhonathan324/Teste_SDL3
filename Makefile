# Build nativo Linux
# Uso: make
# Saída: bin/linux/jogo

CC      := gcc
OUTDIR  := bin/linux
TARGET  := $(OUTDIR)/jogo

# Fontes — usa main.c nativo
SRCS := $(filter-out main_web.c, $(wildcard *.c))

PKG_CONFIG_PATH := /usr/local/lib/pkgconfig
export PKG_CONFIG_PATH

CFLAGS  := -O2 -Wall -Wno-unused-variable \
           $(shell pkg-config --cflags sdl3 sdl3-ttf sdl3-image)

LDFLAGS := $(shell pkg-config --libs sdl3 sdl3-ttf sdl3-image)

.PHONY: all clean run

all: $(OUTDIR) $(TARGET)

$(OUTDIR):
	mkdir -p $(OUTDIR)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDFLAGS) -o $(TARGET)
	@echo "Build concluído: $(TARGET)"

run: all
	cd $(OUTDIR) && LD_LIBRARY_PATH=/usr/local/lib ./jogo

clean:
	rm -rf $(OUTDIR)
