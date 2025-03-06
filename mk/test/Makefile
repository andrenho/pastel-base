PROJECT_NAME = example
PROJECT_VERSION = 1.0.0
include ../config.mk

OBJ = example.o
EMBED = hello.txt

all: $(PROJECT_NAME)

$(OBJ): $(EMBED:=.h)

$(PROJECT_NAME): $(OBJ)
	$(CC) -o $@ $^

leaks: $(PROJECT_NAME)
	$(LEAKS_CMD) ./$<

package: | clean
	tar -cjvf $(PROJECT_NAME)-$(PROJECT_VERSION).tar.bz2 --exclude='.git*' * .??*

.PHONY: clean
clean:
	rm -f example $(OBJ) $(CLEANFILES) $(EMBED:=.h) $(PROJECT_NAME)-*.tar.bz2

.PHONY: install
install: $(PROJECT_NAME)
	install $(PROJECT_NAME) /usr/local/bin