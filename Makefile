CC = g++
CC_FLAGS = -w -g

PROJDIR = $(CURDIR)
SOURCEDIR = $(PROJDIR)/src
CPP_FILES := $(wildcard $(SOURCEDIR)/*.cpp $(SOURCEDIR)/*/*.cpp)
O_FILES = $(CPP_FILES:.cpp=.o)
H_FILES = $(CPP_FILES:.cpp=.h)

LIBS_NAME = -lSDL2 -lSDL2_image
LIBS_PATH = 

PRGM = supertris
BIN_DIR = $(PROJDIR)

all : build

build: $(PRGM)

$(PRGM): $(O_FILES)
	$(CC) $(CC_FLAGS) -o $(PRGM) $(O_FILES) $(LIBS_NAME) $(LIBS_PATH)

%.o: %.c %.h
	$(CC) $(CC_FLAGS) -c $<

.depend:
	$(CC) -MM $(CPP_FILES) > $@

-include .depend

clean:
	rm -f .depend
	rm -f $(O_FILES)
	rm -f $(BIN_DIR)/$(PRGM)

.PHONY: clean
