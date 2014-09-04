CBASE := $(dir $(lastword $(MAKEFILE_LIST)))
CPP := g++
AR  := ar
CURR_PATH := $(shell pwd)

#Include directories
INC += -I$(CBASE)/fatlib/pub-inc -I$(CBASE)/termlib/pub-inc

#Compiler flags
CFLAGS += -g -Wall

#Preprocessor flags
CPPFLAGS += 

#Lib path
LPATH += -L$(CBASE)/fatlib -L$(CBASE)/clilib -L$(CBASE)/termlib 

#Platform specific
ifneq (, $(findstring MinGw, $(PATH)))
    ARCH:=MinGw
    CPPFLAGS+=-DWINDOWS
    LIBCLI_OS_DIR:=windows
else
    ARCH:=Linux
    CPPFLAGS+=-DLINUX
    LIBCLI_OS_DIR:=linux
endif

ifneq (, $(findstring OS-DIR, $(DIRS)))
    DIRS:= $(patsubst OS-DIR, $(LIBCLI_OS_DIR), $(DIRS))
endif

#assemble sources list
SOURCES := $(wildcard *.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(SOURCES))
DEPS:= $(patsubst %.cpp, %.d, $(SOURCES))

#assemble Binary lib depenedencies
BUILT_LIB:=$(shell cat $(CBASE)/.built_libs)

#generic patterns
%.o: %.cpp
	gcc -c $(INC) $(CPPFLAGS) $(CFLAGS) $(LPATH) $*.cpp -o $@
	gcc -MM $(INC) $(CPPFLAGS) $(CFLAGS) $(LPATH) $*.cpp > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

all: amdirs dirs $(LIB) $(BIN) 

amdirs:
	@$(foreach amdir, $(AMDIRS), cd $(amdir) && make -f 3p.mk && cd ..;)
	
dirs:
	@$(foreach dir, $(DIRS), cd $(dir) && make && cd ..;)
	
	
#Create static libraries
$(LIB): $(OBJS)
	$(AR) rcs $(LIB) $(OBJS)
	@echo $(CURR_PATH)/$(LIB) >> $(CBASE)/.built_libs_
	@sort -u $(CBASE)/.built_libs_
	@mv $(CBASE)/.built_libs_ $(CBASE)/.built_libs

$(BIN): $(OBJS) $(BUILT_LIB) 
	$(CPP) $(CPPFLAGS) $(CFLAGS) $(LPATH) $(OBJS) $(LIBS) -o $(BIN)
	
clean: clean-dirs
	@echo "Cleaning up dir $(CURR_PATH)"
	@rm -rf $(OBJS)
	@rm -rf $(LIB)
	@rm -rf $(BIN)
	@rm -rf $(DEPS)

clean-dirs:
	@$(foreach dir, $(DIRS), cd $(dir) && make clean && cd ..;)
	
#try to get a dep file
-include $(DEPS)
	  
