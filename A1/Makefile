CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = myMonitoringTool

SRCS = main.c memory.c cpu.c CLprocessor.c cores.c
OBJS = main.o memory.o cpu.o CLprocessor.o cores.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

memory.o: memory.c
	$(CC) $(CFLAGS) -c memory.c -o memory.o

cpu.o: cpu.c
	$(CC) $(CFLAGS) -c cpu.c -o cpu.o

cores.o: cores.c
	$(CC) $(CFLAGS) -c cores.c -o cores.o

CLprocessor.o: CLprocessor.c
	$(CC) $(CFLAGS) -c CLprocessor.c -o CLprocessor.o

clean:
	rm -f $(OBJS) $(TARGET)