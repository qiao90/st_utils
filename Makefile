DEBUG ?= 1
CC = gcc
ifeq ($(DEBUG),1)
CFLAGS = -g -O2 -DDEBUG
else
CFLAGS = -O2
endif
CPPFLAGS = $(CFLAGS)
CXX = g++
CXXFLAGS = $(CFLAGS)
PACKAGE = st_utils
PACKAGE_NAME = $(PACKAGE)
PACKAGE_STRING = $(PACKAGE_NAME)1.0
PACKAGE_VERSION = 1.0
SHELL = /bin/sh
VERSION = 1.0
SUBDIRS = source
TESTDIR = test
EXTRAFLAGS = -g -I./include -lpthread -lrt -lssl -lcrypto -Wall $(shell mysql_config --cflags --libs )
OBJDIR = obj

vpath %.c $(SUBDIRS)
vpath %.c $(TESTDIR)
vpath %.h   include

srcs =  $(filter-out main.c, $(notdir $(wildcard $(SUBDIRS)/*.c)))
objs = $(srcs:%.c=$(OBJDIR)/%.o)
test_srcs = $(notdir $(wildcard $(TESTDIR)/*.c))
test_objs = $(test_srcs:%.c=$(OBJDIR)/%.o)
test_exec = $(test_srcs:%.c=%)

ifeq ($(DEBUG),1)
TARGET_DIR=Debug
else
TARGET_DIR=Release
endif

all : $(PACKAGE)
.PHONY : all
.PHONY : test

$(PACKAGE) : $(objs) main.c
	@mkdir -p $(TARGET_DIR)
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $(SUBDIRS)/main.c -o $(OBJDIR)/main.o
	$(CC) $(CCFLAGS) $(EXTRAFLAGS) $(objs) $(OBJDIR)/main.o -o $(TARGET_DIR)/$(PACKAGE)

$(objs) : $(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $< -o $@

test : $(objs) $(test_objs)
	@mkdir -p $(TARGET_DIR)
	$(foreach test_target, $(test_exec), $(CC) $(CCFLAGS) $(EXTRAFLAGS) $(objs) -o $(TARGET_DIR)/$(test_target)  $(OBJDIR)/$(test_target).o ;)
$(test_objs) : $(OBJDIR)/%.o: %.c
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $< -o $@

.PHONY : clean
clean :
	-rm -fr $(PACKAGE)
	-rm -fr $(OBJDIR)/*
	-rm -fr $(TARGET_DIR)

