CC = gcc
CFLAGS = -lm -O0 -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow  -fsanitize=address,undefined,leak

TARGET = bplus

SRCS = main.c Bplus.c RH.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean







# CC = gcc
# CFLAGS = -lm -O0 -g -std=c11 -Wall -Werror -Wextra -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable -Wshadow 

# SANFLAGS = -fsanitize=address,undefined,leak

# TARGET = bplus
# SRCS = main.c Bplus.c RH.c
# OBJS = $(SRCS:.c=.o)

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# debug: CFLAGS += $(SANFLAGS)
# debug: clean $(TARGET)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $< -o $@

# run: $(TARGET)
# 	./$(TARGET)

# valgrind: $(TARGET)
# 	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# clean:
# 	rm -f $(OBJS) $(TARGET)

# .PHONY: all run clean debug valgrind