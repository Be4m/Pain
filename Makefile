CC=gcc
MD=mkdir

INCLUDE=-Iinclude -Iinclude/vendor
LIB=-Llib -lglfw3 -lgdi32 -lglew32s -lopengl32
CFLAGS=$(INCLUDE) -std=c99 -Wall -DGLEW_STATIC -g -D_DEBUG
LDFLAGS=$(LIB) -mwindows

SRC=src/main.c src/application.c \
	src/graphics.c src/shaders.c src/shader_store.c \
	src/vendor/stb_image.c

SHADERS=shaders/standard_texture.frag shaders/standard_texture.vert \
	shaders/standard.frag shaders/standard.vert

SHADER_OBJ=$(patsubst shaders/%, obj/shaders/%.o, $(SHADERS))
OBJ=$(patsubst src/%.c, obj/%.o, $(SRC)) $(SHADER_OBJ)
	

all: pain.exe
remake: clean all

obj/%.o: src/%.c
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	$(CC) $(CFLAGS) -c $< -o $@

obj/shaders/%.o obj/shaders/%.o: shaders/%
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	ld -r -b binary -o $@ $^

pain.exe: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rmdir /s /q obj
	del pain.exe

.PHONY: clean remake