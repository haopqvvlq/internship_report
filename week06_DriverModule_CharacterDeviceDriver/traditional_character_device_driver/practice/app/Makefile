CC := gcc

TARGET := app

SRC := test.c

OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

CFLAGS := -Wall -Wextra -std=c11 -I../include -MMD -MP

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

-include $(DEP)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)