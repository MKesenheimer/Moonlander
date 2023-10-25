########################################################################
#                          -*- Makefile -*-                            #
########################################################################
# Force rebuild on these rules
.PHONY: all libs clean clean-libs
.DEFAULT_GOAL := moonlander

#COMPILER = /opt/local/bin/g++
COMPILER = /usr/bin/g++

########################################################################
## Flags
FLAGS   = -g -std=c++17 -DLUMAX_OUTPUT
## find shared libraries during runtime: set rpath:
LDFLAGS = -rpath @executable_path/libs
PREPRO  =
##verbose level 1
#DEBUG   = -D DEBUGV1
##verbose level 2
#DEBUG  += -D DEBUGV2
##verbose level 3
#DEBUG  += -D DEBUGV3
OPT     = -O2
WARN    = -Wall -Wno-missing-braces

### generate directory obj, if not yet existing
$(shell mkdir -p build)

########################################################################
## Paths

WORKINGDIR = $(shell pwd)
PARENTDIR  = $(WORKINGDIR)/..
SYSTEMINC  = /opt/local/include
LIBS       = $(WORKINGDIR)/libs
#EIGEN      = $(LIBS)/eigen
#GTEST      = $(LIBS)/googletest
LIBLUMAX   = $(LIBS)/lumax
#LIBGTEST   = $(LIBS)/googletest/build/lib

########################################################################
### DO NOT MODIFY BELOW THIS LINE ######################################
########################################################################

########################################################################
## search for the files and set paths

vpath %.cpp $(WORKINGDIR) $(WORKINGDIR)/GameLibrary
vpath %.m $(WORKINGDIR)
vpath %.a $(WORKINGDIR)/build
vpath %.o $(WORKINGDIR)/build

########################################################################
## Includes
CXX  = $(COMPILER) $(FLAGS) $(OPT) $(WARN) $(DEBUG) $(PREPRO) -I$(SYSTEMINC) -I$(WORKINGDIR) -I$(LIBS)
INCLUDE = $(wildcard *.h $(UINCLUDE)/*.h)

########################################################################
## libraries
### SDL
CXX += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs) -lSDL2_gfx -lSDL2_image

### Lumax
# TODO: make this OS dependent
LDFLAGS += -L$(LIBLUMAX)/libs -llumax_darwin 

# Frameworks
# -framework SDL_gfx 
#FRM = -framework Cocoa

### Unittests
#LDFLAGS_U = $(LDFLAGS)
#LDFLAGS_U += -L$(LIBGTEST) -lgtest

########################################################################
## Build rules
%.a: %.cpp $(INCLUDE)
	$(CXX) -c -o build/$@ $<

%.a: %.m $(INCLUDE)
	$(CXX) -c -o build/$@ $<

########################################################################
## BUILD Files
BUILD = main.a object.a lander.a fuelbar.a character.a charstring.a
BUILD+= moon.a collision.a part.a renderer.a

########################################################################
## Rules
## type make -j4 [rule] to speed up the compilation
all: libs moonlander

moonlander: $(BUILD)
	  $(CXX) $(patsubst %,build/%,$(BUILD)) $(LDFLAGS) $(FRM) -o $@

libs:
	cd $(LIBLUMAX) && ./make.sh macOS
	cp $(LIBLUMAX)/libs/*.dylib $(LIBS)

clean-all: clean clean-libs

clean:
	rm -f build/*.a moonlander

clean-libs:
	cd $(LIBLUMAX) && rm -f *.dylib *.so
	rm -f $(LIBS)/*.dylib $(LIBS)/*.so

do:
	make && ./moonlander

########################################################################
#                       -*- End of Makefile -*-                        #
########################################################################
