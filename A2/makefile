CC = gcc
CFLAGS = -Wall -std=c11 -D_POSIX_C_SOURCE=200809L

TARGET = showFDtables

SRCS = main.c argumentHandler.c composite.c outputHandler.c perProcess.c summary.c systemWide.c threshold.c Vnodes.c getInfo.c

OBJS = $(SRCS:.c=.o)

## Compile the .c files to make the executable.
all: $(TARGET)

## Link all the .o files to the executable.
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

## Compile each .c file to .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

## Clean all .o files and showFDtables
clean:
	rm -f $(OBJS) $(TARGET)

help: makefile
	@sed -n 's/^## //p' $<

.PHONY: all clean help