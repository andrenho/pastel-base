# Variables that can be set:
#   RELEASE=1        create a release build
#   PROJECT_VERSION

#
# compiler configuration
#

CFLAGS += -std=c17
CPPFLAGS += -MMD -MP   # generate dependencies
CXXFLAGS += -std=c++23

ifdef $(RELEASE)
	CPPFLAGS = -Ofast -flto
else
	CPPFLAGS += -Wall -Wextra -Wformat-nonliteral -Wshadow -Wwrite-strings -Wmissing-format-attribute -Wswitch-enum -Wmissing-noreturn -Wno-unused-parameter -Wno-unused
	ifeq ($(CXX),gcc)
		CPPFLAGS += -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=malloc -Wsuggest-attribute=format -Wsuggest-attribute=cold
	endif
	CPPFLAGS += -ggdb -O0
	CPPFLAGS += -DPROJECT_VERSION=\"$(PROJECT_VERSION)\"
endif

#
# generate embedded files (require LuaJIT)
#

CONFIG_MK_DIR:= $(dir $(lastword $(MAKEFILE_LIST)))
GENHEADER := $(CONFIG_MK_DIR)/genheader.lua
%.h: %
	$(GENHEADER) $^ > $@
.DELETE_ON_ERROR=%.h

#
# generate rule dependencies
#

DEPENDS = $(shell find . -name *.d)
-include $(DEPENDS)
CLEANFILES := $(DEPENDS) libluajit.a

#
# leak check command
#

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)  # Apple
	LEAKS_CMD := MallocStackLogging=1 leaks --atExit --
	MACOS_VERSION := $(shell cut -d '.' -f 1,2 <<< $$(sw_vers -productVersion))
else
	LEAKS_CMD := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --fair-sched=yes --suppressions=valgrind.supp
endif

#
# rule to print config
#

all: $(PROJECT_NAME)

.PHONY: config
config:
	@echo ===============================
	@echo CC            = $(CC)
	@echo CXX           = $(CXX)
	@echo CFLAGS        = $(CFLAGS)
	@echo CPPFLAGS      = $(CPPFLAGS)
	@echo CXXFLAGS      = $(CXXFLAGS)
	@echo LDFLAGS       = $(LDFLAGS)
	@echo DEPENDS       = $(DEPENDS)
	@echo CONFIG_MK_DIR = $(CONFIG_MK_DIR)
	@echo ===============================

%: | config
	@true

deepclean:
	git clean -fdx

update:
	git submodule update --init --remote --merge

#
# LuaJIT
#

libluajit.a:
	$(MAKE) -C $(CONFIG_MK_DIR)/LuaJIT/src MACOSX_DEPLOYMENT_TARGET="${MACOS_VERSION}" libluajit.a
	cp $(CONFIG_MK_DIR)/LuaJIT/src/libluajit.a .

clean-lua:
	$(MAKE) -C $(CONFIG_MK_DIR)/LuaJIT MACOSX_DEPLOYMENT_TARGET="${MACOS_VERSION}" clean