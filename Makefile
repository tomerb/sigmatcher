# 1. Compile every *.cpp in the folder
# 2. All obj files under obj folder
# 3. static library .a at lib folder
# 4. run 'make dirmake' before calling 'make'


CC = g++
OUT_FILE_NAME = libsigmatcher.a

CFLAGS = -O3 -g -Wall -Werror -Wextra -c -fPIC

SMHASHER_DIR = libs/smhasher

SRC_DIR = ./src
INC = -I$(SRC_DIR) -I$(SMHASHER_DIR)/src/

OBJ_DIR=./obj

OUT_DIR=./lib

TEST_DIR=./test/unit

# Enumerating of every *.cpp as *.o and using that as dependency.
# filter list of .c files in a directory.
# FILES =dump_l.c \
#	kter.c \
#
# $(OUT_FILE_NAME): $(patsubst %.c,$(OBJ_DIR)/%.o,$(wildcard $(FILES)))


# Enumerating of every *.cpp as *.o and using that as dependency
$(OUT_FILE_NAME): $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp))
	ar -r -o $(OUT_DIR)/$@ $^

#Compiling every *.cpp to *.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp dirmake smhasher
	$(CC) -c $(INC) $(CFLAGS) -o $@  $<

dirmake:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)

smhasher:
	@mkdir -p $(SMHASHER_DIR)/build
	@cd $(SMHASHER_DIR)/build && cmake ../src && make -j4

test: $(OUT_FILE_NAME)
	@make -C $(TEST_DIR) run

clean:
	rm -rf $(OBJ_DIR)/*.o $(OUT_DIR)/$(OUT_FILE_NAME) Makefile.bak $(SMHASHER_DIR)/build
	@make -C $(TEST_DIR) veryclean

rebuild: clean build
