CC = gcc
CFLAGS = -Wall -I./include -g -DDEBUG_SEMANTIC

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/compiler

$(shell mkdir $(OBJ_DIR))
$(shell mkdir $(BIN_DIR))

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

debug_lexer: CFLAGS += -DDEBUG_LEXER
debug_lexer: clean all

debug_parser: CFLAGS += -DDEBUG_PARSER
debug_parser: clean all

debug_semantic: CFLAGS += -DDEBUG_SEMANTIC
debug_semantic: clean all

debug_all: CFLAGS += -DDEBUG_LEXER -DDEBUG_PARSER -DDEBUG_SEMANTIC
debug_all: clean all

clean:
	rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all debug_lexer debug_parser debug_semantic debug_all clean