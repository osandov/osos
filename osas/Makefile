CC := gcc
CFLAGS := -Wall -g -I..

BUILD ?= build
LIBBUILD ?= ../osos/$(BUILD)

SRCS := Assemble.c \
	Buffer.c \
	Error.c \
	Labels.c \
	Lexer.c \
	Output.c \
	Passes.c \
	main.c

OBJS := $(addprefix $(BUILD)/, $(SRCS:.c=.o))

$(BUILD)/osas: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBBUILD) -losos -Wl,-rpath,.

$(BUILD)/%.o : %.c | $(BUILD)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)
