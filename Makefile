CXX = clang++
CFLAGS = -Wall -g -O3

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

TARGET = bin/Ising

all: $(TARGET)

$(TARGET): build $(OBJECTS) 
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET)

build:
	@mkdir -p bin

PHONY .clean:
clean:
	@-rm -rf src/*.o
	@if [ -d bin ] ; \
		then rm -rf bin; \
	fi
