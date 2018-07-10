CC = g++
INCLUDES = -Iinclude
C = -x c
CPP = -x c++
OUT = -o

MKDIR = mkdir -p

MAIN_DIR = build/main


main:
	$(MKDIR) $(MAIN_DIR)
	$(CC) $(INCLUDES) src/main.cpp $(OUT) $(MAIN_DIR)/main
