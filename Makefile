.PHONY: all clean
PROJECT := Network_Info
CUR_DIR := .

INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
OBJ_DIR := $(CUR_DIR)/obj
BIN_DIR := $(CUR_DIR)/bin
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC := $(LIB_DIR)/static

INC_FLAGS := -I $(INC_DIR)
CC :=gcc

objs:
	$(CC)  -c -o $(OBJ_DIR)/Network_Info.o $(SRC_DIR)/Network_Info.c  $(INC_FLAGS)
	$(CC)  -c -o $(OBJ_DIR)/main.o main.c $(INC_FLAGS)

mkstatic: objs
	ar rcs $(LIB_STATIC)/libNetwork_Info.a $(OBJ_DIR)/Network_Info.o
all: mkstatic
	$(CC)  $(OBJ_DIR)/main.o  -L$(LIB_STATIC) -l$(PROJECT) -o $(BIN_DIR)/$(PROJECT)
clean:
	rm -rf $(OBJ_DIR)/* $(LIB_STATIC)/libNetwork_Info.a $(BIN_DIR)/$(PROJECT)
