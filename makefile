CC = g++
INCLUDES = -Iinclude
C = -x c
CPP = -x c++
OUT = -o
STB = include/stb_image.cpp

MKDIR = mkdir -p

MAIN_DIR = build/main

SRC = src/main.cpp
DEST = build/main
EXEC = main

OPEN = xdg-open

OUT_DIR = build/main
OUT_FILE = out.ppm


COMPILE = $(CC) $(INCLUDES) $(STB) $(IN_SRC) $(SRC) $(OUT) $(DEST)/$(EXEC)

main:
	$(MKDIR) $(DEST)
	$(COMPILE)
.PHONY: main

run_main:
	./build/main/main $(OUT_DIR)/$(OUT_FILE)
	$(OPEN) $(OUT_DIR)/$(OUT_FILE)


sphere:
	$(eval SRC := src/simple_sphere.cpp)
	$(eval DEST := build/sphere)
	$(eval EXEC := sphere)
	$(MKDIR) $(DEST)
	$(COMPILE)
.PHONY: sphere

run_sphere:
	$(eval OUT_FILE := build/sphere/out.ppm)
	./build/sphere/sphere $(OUT_FILE)
	$(OPEN) $(OUT_FILE)

clean:
	rm -r build/*
