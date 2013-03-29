LINKERNAME := libosos.so
SONAME := $(LINKERNAME).0
REALNAME := $(SONAME).0

BUILD ?= build
BIN ?= bin

all:
	make -C osos BUILD=$(BUILD) LINKERNAME=$(LINKERNAME) \
	SONAME=$(SONAME) REALNAME=$(REALNAME)
	make -C osvm BUILD=$(BUILD)
	make -C osas BUILD=$(BUILD)

install: all
	mkdir -p $(BIN)
	cp osos/$(BUILD)/$(REALNAME) osvm/$(BUILD)/osvm osas/$(BUILD)/osas $(BIN)/
	ln -sf $(REALNAME) $(BIN)/$(SONAME)
	ln -sf $(SONAME) $(BIN)/$(LINKERNAME)

doc:
	doxygen

clean:
	make -C osos clean
	make -C osvm clean
	make -C osas clean
	rm -rf $(BIN)
