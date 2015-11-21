CC = gcc
CFLAGS = -g -O2
CPPFLAGS = 
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
EXTRAFLAGS = -g -I./include -lpthread -lrt -lssl -lcrypto -Wall
OBJDIR = obj

vpath %.c $(SUBDIRS)
vpath %.c $(TESTDIR)
vpath %.h   include

srcs =  $(filter-out main.c, $(notdir $(wildcard $(SUBDIRS)/*.c)))
objs = $(srcs:%.c=$(OBJDIR)/%.o)
test_srcs = $(notdir $(wildcard $(TESTDIR)/*.c))
test_objs = $(test_srcs:%.c=$(OBJDIR)/%.o)
test_exec = $(test_srcs:%.c=%)

all : $(PACKAGE)
.PHONY : all
.PHONY : test

$(PACKAGE) : $(objs) main.c
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $(SUBDIRS)/main.c -o $(OBJDIR)/main.o
	$(CC) $(CCFLAGS) $(EXTRAFLAGS) $(objs) $(OBJDIR)/main.o -o $(PACKAGE) 

$(objs) : $(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $< -o $@ 

test : $(test_objs)
	$(foreach test_target, $(test_exec), $(CC) $(CCFLAGS) $(EXTRAFLAGS) $(objs) -o $(test_target)  $(OBJDIR)/$(test_target).o ;)	
$(test_objs) : $(OBJDIR)/%.o: %.c
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $< -o $@ 

.PHONY : clean 
clean :	
	-rm -fr $(PACKAGE)
	-rm -fr $(OBJDIR)
	-rm -fr $(test_exec) 
