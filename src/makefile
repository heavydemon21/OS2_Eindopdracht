CC = g++
LD = g++
RM = rm -f
CFLAGS =
LFLAGS =
TARGET = main

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o, $(SRCS))

all: $(TARGET)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@

$(TARGET): $(OBJS)
	$(LD) $^ $(LFLAGS) -o $@

clean:
	$(RM) $(TARGET) $(OBJS)

compile_commands:
	compiledb make -Bn