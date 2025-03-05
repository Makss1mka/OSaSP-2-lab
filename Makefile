CC = gcc
CFLAGS = -Wall -Wextra -Werror  -Wformat -pedantic

TARGETS = parent child

all: $(TARGETS)

parent: parent.c 
    $(CC) $(CFLAGS) -o parent parent.c

child: child.c
    $(CC) $(CFLAGS) -o child child.c

clean:
    rm -f $(TARGETS)
