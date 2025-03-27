CC = gcc
CFLAGS = -Wall -Werror -g

TARGET = myMonitoringTool

SRCS = main.c memory.c cpu.c cores.c CLprocessor.c signalsprocessor.c
OBJS = $(SRCS:.c=.o)

## Compile the .c files to make the executable.
all: $(TARGET)

## Link all the .o files to the executable.
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

## Compile each .c file to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

## Clean all object files and myMonitoringTool
clean:
	rm -f $(OBJS) $(TARGET)

help: Makefile
	@sed -n 's/^## //p' $<

.PHONY: all clean help