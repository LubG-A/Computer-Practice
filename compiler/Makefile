# 编译器和选项
CC = gcc
CFLAGS = -Wall -I./include

# 源文件和目标文件
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# 自动获取所有.c文件
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = $(BIN_DIR)/compiler

# 创建必要的目录
$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(BIN_DIR))

# 默认目标
all: $(TARGET)

# 编译所有源文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@

# 链接生成可执行文件
$(TARGET): $(OBJS)
    $(CC) $(OBJS) -o $@

# 启用词法分析器调试
debug_lexer: CFLAGS += -DDEBUG_LEXER
debug_lexer: clean all

# 启用语法分析器调试
debug_parser: CFLAGS += -DDEBUG_PARSER
debug_parser: clean all

# 同时启用两种调试
debug_all: CFLAGS += -DDEBUG_LEXER -DDEBUG_PARSER
debug_all: clean all

# 清理生成的文件
clean:
    rm -f $(OBJ_DIR)/*.o $(TARGET)

.PHONY: all debug_lexer debug_parser debug_all clean