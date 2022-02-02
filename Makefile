TARGET = example
LIBS = -lcurl -ljsoncpp
CC = g++

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst src/%.cpp, src/%.o, $(wildcard src/*.cpp) *.cpp)
HEADERS = $(wildcard src/*.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f src/*.o
	-rm -f $(TARGET)
