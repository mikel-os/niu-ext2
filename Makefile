# This is a draft.
# 	The program is a library, but is not yet set as target.
# 	The "ext2.elf" is not considered a rigorous test, and must disappear.
#	In its place, "./test" will contain tests. Its executable can be hosted
# 	  on $(TARGET_DIR).

CC=gcc
TARGETS_NAMES=ext2.elf
OBJS_NAMES=main superblock
FILE=filedumps/superblocks/sfa
DEBUG=-g
CFLAGS=-Wall -Wextra

INCLUDE=./include
OBJ=./obj
TARGET_DIR=./build

TARGETS=$(addprefix $(TARGET_DIR)/, $(TARGETS_NAMES))
OBJS=$(addprefix $(OBJ)/, $(addsuffix .o, $(OBJS_NAMES)))

build: $(TARGETS)

clean:
	rm -vf $(OBJ)/*.o

rebuild: clean build

nuke: clean
	rm -vf $(TARGET_DIR)/lib*.a
	rm -vf $(TARGET_DIR)/*.elf

nukerebuild: nuke build

help:
	@echo "To do"

# This pair of rules and the FILE variable are temporal, don't worry about them.
launch: $(TARGET_DIR)/ext2.elf
	$< < $(FILE)

all: rebuild launch

nukefirst: nukerebuild lauch

debug: $(TARGET_DIR)/ext2.elf
	gdb $<

.PHONY: build rebuild clean nuke nukebuild help
.PHONY: launch all nukefirst debug

$(TARGET_DIR)/ext2.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ)/%.o: ./src/%.c
	$(CC) -c $(CFLAGS) $(DEBUG) -I$(INCLUDE) -o $@ $<

