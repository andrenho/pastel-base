PROJECT_NAME = example
PROJECT_VERSION = 1.0.0

all: example

include ../config.mk

OBJ = example.o
EMBED = hello.txt

$(UI_OBJ): $(EMBED:=.h)
$(UI_OBJ): libluajit.a

$(PROJECT_NAME): $(UI_OBJ) libluajit.a libSDL3.a
	$(CC) -o $@ $^ $(LDFLAGS)

leaks: $(PROJECT_NAME)
	$(LEAKS_CMD) ./$<

package: | clean
	tar -cjvf $(PROJECT_NAME)-$(PROJECT_VERSION).tar.bz2 --exclude='.git*' * .??*

.PHONY: clean
clean:
	rm -f example $(UI_OBJ) $(CLEANFILES) $(EMBED:=.h) $(PROJECT_NAME)-*.tar.bz2 *.a

.PHONY: install
install: $(PROJECT_NAME)
	install $(PROJECT_NAME) /usr/local/bin
