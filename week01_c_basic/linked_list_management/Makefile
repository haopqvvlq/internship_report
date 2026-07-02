CC := gcc

TARGET := app

SRC := main.c \
       lib/quanly.c

OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

CFLAGS := -Wall -Wextra -std=c11 -Ilib -MMD -MP

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

-include $(DEP)

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)