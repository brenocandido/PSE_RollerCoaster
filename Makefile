FLAGS := -Wall -std=c++20 -pthread
TARGET := main
ARGS :=

SOURCES := $(shell find -name "*.cpp")
DEPENDENCIES := $(patsubst %.cpp,%.d,$(SOURCES))

all: $(TARGET)

%.o: %.cpp $(DEPENDENCIES)
	@$ g++ -c -o $@ $< $(FLAGS)

$(TARGET): $(SOURCES)
	@$ g++ -o $@ $^ $(FLAGS)

run: $(TARGET)
	./$(TARGET) $(ARGS)

clean:
	@-rm -f *.o $(TARGET)

