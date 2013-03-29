CC := gcc
CFLAGS := -Wall -g -I..

BUILD ?= build
LIBBUILD ?= ../osos/$(BUILD)

SRCS := osvm.c

OBJS := $(addprefix $(BUILD)/, $(SRCS:.c=.o))

$(BUILD)/osvm: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBBUILD) -losos -Wl,-rpath,.

$(BUILD)/%.o : %.c | $(BUILD)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)
