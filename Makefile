CC      := x86_64-elf-gcc
AR      := x86_64-elf-ar

ifeq (, $(shell which $(CC) 2>/dev/null))
  CC := gcc
  AR := ar
endif

CFLAGS := \
    -std=c11 \
    -ffreestanding \
    -fno-stack-protector \
    -fno-pic \
    -mno-red-zone \
    -mno-mmx -mno-sse -mno-sse2 \
    -Wall -Wextra -O2 \
    -Iinclude

SRCS := src/zirvflux.c
OBJS := build/zirvflux.o
TARGET := libzirvflux.a

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) rcs $@ $^

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf build $(TARGET)
