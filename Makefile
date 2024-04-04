CC=gcc
INCLUDE=-Iinclude -Iinclude/vendor
LIB=-Llib -lglfw3 -lgdi32 -lglew32s -lopengl32
CFLAGS=$(INCLUDE) -std=c99 -Wall -DGLEW_STATIC -g -D_DEBUG
LDFLAGS=$(LIB)

SRC=src/main.c src/application.c \
	src/graphics.c src/shaders.c src/shader_store.c \
	src/vendor/stb_image.c
OBJ=$(patsubst src/%.c, obj/%.o, $(SRC))

all: pain.exe

remake: clean all

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

pain.exe: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	del /s *.o pain.exe

.PHONY: clean remake