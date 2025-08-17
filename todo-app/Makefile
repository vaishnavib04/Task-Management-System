CC=gcc
CFLAGS=`pkg-config --cflags gtk+-3.0` -Wall
LIBS=`pkg-config --libs gtk+-3.0` -lgdi32 -Wl,-subsystem,console
SRC=src/main.c
TARGET=build/todo-app

$(TARGET): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)