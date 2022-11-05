CC = gcc

CFLAGS = -pipe -std=gnu17 -Wpedantic -Wall -Wextra # -Werror
LDLIBS = -pthread

LEXYACC_PATH = ./gen
INCLUDE_PATH = ./inc
LIB_PATH     = ./lib
VM_PATH      = ./vm

TARGET       = sos
FILEXT       = c

SRCDIR       = src
OBJDIR       = obj
BINDIR       = bin

SOURCES     := $(wildcard $(SRCDIR)/*.$(FILEXT))
INCLUDES    := $(wildcard $(INCLUDE_PATH)/*.h)
LIBS        := $(wildcard $(LIB_PATH)/*.h|*.hpp)
OBJECTS0     := $(SOURCES:$(SRCDIR)/%.$(FILEXT)=$(OBJDIR)/%.o)

LEXSRC      := $(wildcard $(LEXYACC_PATH)/*.l)
YACCSRC     := $(wildcard $(LEXYACC_PATH)/*.y)
LEXC				:= $(LEXSRC:$(LEXYACC_PATH)/%.l=$(SRCDIR)/%.c)
YACCC				:= $(YACCSRC:$(LEXYACC_PATH)/%.y=$(SRCDIR)/%.c)
LEXOBJ			:= $(LEXSRC:$(LEXYACC_PATH)/%.l=$(OBJDIR)/%.o)
YACCOBJ			:= $(YACCSRC:$(LEXYACC_PATH)/%.y=$(OBJDIR)/%.o)

OBJECTS      = $(filter-out $(LEXOBJ) $(YACCOBJ), $(OBJECTS0))

PATH_TO_EXE  = $(BINDIR)/$(TARGET)
LAUNCH_CMD   = $(PATH_TO_EXE) -i ./examples/hello_world.sos

all : debug

debug: CFLAGS += -Og -DDEBUG -g -ggdb
debug: $(PATH_TO_EXE)
	@echo "\033[93mRunning in debug mode!\033[0m"

release: CFLAGS += -march=native -Ofast
release: $(PATH_TO_EXE)
	@echo "\033[96mRunning in release mode!\033[0m"

generic: CFLAGS += -march=x86-64 -Ofast
generic: $(PATH_TO_EXE)
	@echo "\033[95mRunning in generic mode!\033[0m"

run:
ifneq ("$(wildcard $(PATH_TO_EXE))", "")
	./$(LAUNCH_CMD)
else
	@echo "\033[91mNo executable found!\033[0m"
endif

run-release: release
	./$(LAUNCH_CMD)

run-debug: debug
	valgrind --leak-check=full --show-leak-kinds=all --vgdb=full -s ./$(LAUNCH_CMD)


$(LEXC):
	flex -o $@ $(LEXSRC)

$(YACCC):
	bison -d -o $@ $(YACCSRC)

$(PATH_TO_EXE): $(OBJECTS) $(YACCOBJ) $(LEXOBJ)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)
	@echo "\033[92mLinking complete!\033[0m"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC) $(LEXC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(LEXOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(LEXC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)

$(YACCOBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.$(FILEXT) $(INCLUDES) $(YACCC)
	mkdir -p $(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS) -isystem$(INCLUDE_PATH) -isystem$(LIB_PATH)


.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(OBJDIR)/*.gcda
	rm -f $(OBJDIR)/*.gcno
	rm -f $(PATH_TO_EXE)
	rm -f $(LEXC)
	rm -f $(YACCC) $(YACCC:.c=.h)
