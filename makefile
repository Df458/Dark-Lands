PKGCONFIG=pkg-config
OS=GNU/Linux
CFLAGS=-I../src -O3 -pipe -g -pg -Wall -Wno-unused-variable -pedantic-errors `$(PKGCONFIG) --cflags lua ncurses`
WINFLAGS=-Iinclude -Wl,-subsystem,windows -static-libgcc -static-libstdc++ -DWINDOWS
LINUXFLAGS=
LIBS=-L. -Wl,-rpath -Wl,./lib
WINLIBS=`$(PKGCONFIG) --libs --static lua`
LINUXLIBS=`$(PKGCONFIG) --libs lua ncurses`
SRCPATH=src/
HTMLPATH=html/
OBJPATH=obj/
ENGINESRCS:=$(wildcard $(SRCPATH)*.c)
ENGINEOBJS:=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o,$(ENGINESRCS))
ENGINEDEPS:=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.depend,$(ENGINESRCS))
HTML:=$(patsubst $(SRCPATH)%.c,$(HTMLPATH)%.html,$(ENGINESRCS))
ENGINEAPP=darklands
ifeq ($(OS), GNU/Linux)
OSFLAGS=$(LINUXFLAGS)
OSLIBS=$(LINUXLIBS)
else ifeq ($(OS), Windows)
OSFLAGS=$(WINFLAGS)
OSLIBS=$(WINLIBS)
CC=i686-w64-mingw32-gcc
PKGCONFIG=i686-w64-mingw32-pkg-config
ENGINEAPP := $(ENGINEAPP).exe
else
$(error Unknown OS selected for output)
endif

$(OBJPATH)%.depend: $(SRCPATH)%.c
	@echo -e "Building dependecies for \e[1;35m$<\e[0m..."
	@set -e; rm -f $@; \
	$(CC) -M $(CFLAGS) $(OSFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,obj/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(OBJPATH)%.o: $(SRCPATH)%.c
	@echo -e "Building \e[1;35m$<\e[0m..."
	@$(CC) -c $< -o $@ $(CFLAGS) $(OSFLAGS)

$(HTMLPATH)%.html: $(SRCPATH)%.c
	highlight -la -o $@ $<

all: $(ENGINEDEPS) $(ENGINESRCS) $(ENGINEOBJS) $(HTML)
	@echo -e "\e[0;33mBuilding main application for \e[1;35m$(OS)\e[0;33m systems...\e[0m"
	@$(CC) -o $(ENGINEAPP) $(ENGINEOBJS) $(CFLAGS) $(OSFLAGS) $(LIBS) $(OSLIBS)
	@echo -e "\e[0;32mBuild completed.\e[0m"

release: all
	@cp -r data bin
	@cp $(ENGINEAPP) bin

-include $(ENGINEDEPS)
$(shell   mkdir -p obj)
$(shell   mkdir -p bin)

.PHONY: clean
clean:
	@echo -e "\e[0;31mCleaning up...\e[0m"
	@$(RM) $(ENGINEOBJS)
	@$(RM) $(OBJPATH)*.depend
	@$(RM) $(OBJPATH)*.depend.*
	@$(RM) $(ENGINEAPP)
