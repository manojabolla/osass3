CC     = gcc
CFLAGS = -g
TARGET = oss user
all: $(TARGET)
OBJS   = oss.o user.o
#.SUFFIXES: .c .o

oss: oss.o
	$(CC) -o oss oss.o -pthread

user: user.o
	$(CC) -o user user.o -pthread

.c.o:
	$(CC) $(CFLAGS) -c $<
clean:
	/bin/rm -f *.o $(TARGET)

