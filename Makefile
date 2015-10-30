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
EXTRAFLAGS = -g -I./include -lpthread 
OBJDIR = obj

vpath %.c $(SUBDIRS)
vpath %.h   include

srcs = $(notdir $(wildcard $(SUBDIRS)/*.c))
objs = $(srcs:%.c=$(OBJDIR)/%.o)

all: $(PACKAGE)
.PHONY : all

$(PACKAGE) : $(objs)
	$(CC) $(CCFLAGS) $(EXTRAFLAGS) $(objs) -o $(PACKAGE) 

$(objs) : $(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) -c $(CCFLAGS) $(EXTRAFLAGS) $< -o $@ 


.PHONY : clean 
clean :	
	-rm -fr $(PACKAGE)
	-rm -fr $(OBJDIR) 
