CC=gcc
MD=mkdir

INCLUDE=-Iinclude -Iinclude/vendor
LIB=-Llib -lglfw3 -lgdi32 -lglew32s -lopengl32
CCFLAGS=$(INCLUDE) -std=c99 -Wall -DGLEW_STATIC
LDFLAGS=$(LIB)

EXE=pain.exe
BUILD_DIR=build

SRC=src/main.c src/application.c \
	src/graphics.c src/shaders.c src/shader_store.c \
	src/camera.c \
	src/shapes.c \
	src/vendor/stb_image.c

SHADERS=shaders/standard_texture.frag shaders/standard_texture.vert \
	shaders/standard.frag shaders/standard.vert \
	shaders/simple_lighting.frag shaders/simple_lighting.vert \
	shaders/simple.frag

SHADER_OBJS=$(patsubst shaders/%, obj/shaders/%.o, $(SHADERS))
OBJS=$(patsubst src/%.c, obj/%.o, $(SRC)) $(SHADER_OBJS)

DBGDIR=$(addsuffix /debug, $(BUILD_DIR))
DBGEXE=$(DBGDIR)/$(EXE)
DBGOBJS=$(addprefix $(DBGDIR)/, $(OBJS))
DBGCCFLAGS=-g -D_DEBUG

RELDIR=$(addsuffix /release, $(BUILD_DIR))
RELEXE=$(RELDIR)/$(EXE)
RELOBJS=$(addprefix $(RELDIR)/, $(OBJS))
RELCCFLAGS=-O3
RELLDFLAGS=-mwindows


.PHONY: clean remake

all: debug

remake: clean all
run: all
	$(DBGEXE)

clean:
	rmdir /s /q $(BUILD_DIR)
	del $(EXE)

# == DEBUG ==
debug: $(DBGEXE)

$(DBGEXE): $(DBGOBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(DBGDIR)/obj/%.o: src/%.c
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	$(CC) $(CCFLAGS) $(DBGCCFLAGS) -c $< -o $@

$(DBGDIR)/obj/shaders/%.o: shaders/%
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	ld -r -b binary -o $@ $^

# == RELEASE ==
release: $(RELEXE)

$(RELEXE): $(RELOBJS)
	$(CC) -o $@ $^ $(LDFLAGS) $(RELLDFLAGS)

$(RELDIR)/obj/%.o: src/%.c
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	$(CC) $(CCFLAGS) $(RELCCFLAGS) -c $< -o $@

$(RELDIR)/obj/shaders/%.o: shaders/%
	-@ if NOT EXIST "$(@D)" $(MD) "$(@D)"
	ld -r -b binary -o $@ $^