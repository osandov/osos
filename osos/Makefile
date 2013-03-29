CC := gcc
CFLAGS := -Wall -fPIC -g -I..

BUILD ?= build

SRCS := Instructions.c \
    	OpCodes.c \
    	VirtualMachine.c

OBJS := $(addprefix $(BUILD)/, $(SRCS:.c=.o))

$(BUILD)/$(REALNAME): $(OBJS)
	$(CC) $(CFLAGS) -shared -Wl,-soname,$(SONAME) -o $@ $^
	ln -sf $(REALNAME) $(BUILD)/$(SONAME)
	ln -sf $(SONAME) $(BUILD)/$(LINKERNAME)

$(BUILD)/%.o : %.c | $(BUILD)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILD):
	mkdir $(BUILD)

clean:
	rm -rf $(BUILD)
