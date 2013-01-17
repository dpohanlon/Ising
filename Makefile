CXX = clang++
CPPFLAGS = -Wall -g -O3
CFLAGS = -Wall -g -O3

INCLUDES = -I /usr/local/Cellar/boost/1.51.0/
LIBS = -lboost_thread-mt -lboost_system-mt

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

TARGET = bin/Ising

all: $(TARGET)

$(TARGET): build $(OBJECTS)
	$(CXX) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LIBS)

build:
	@mkdir -p bin

.PHONY: clean
clean:
	@-rm -rf src/*.o
	@if [ -d bin ] ; \
		then rm -rf bin; \
	fi
