
# MACRO TO CONVERT src/%.c TO obj/%.o OR  src/%.cpp to obj/%.o
define TO
$(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(patsubst $(SRC)%,$(OBJ)%,$(1))))
endef

define COMPILE
$(5) : $(3)
	$(1) $(2) -c $(3) $(4) -o $(5)
endef


APP     := draw
CC		:= g++
CFLAGS  := -std=c++20 -Wall
Q       := question4

SRC	    := src
OBJ     := obj
LIB     := lib
INCLUDE := -I $(SRC) -I $(LIB)
STCKLIBS := $(shell find $(LIB) -type f -iname *.a)
LIBS     := $(STCKLIBS) -ll -lX11

# SCANNER AND PARSER ABOUT QUESTION N ##########################
SCANFILE  := $(shell find $(SRC)/$(Q) -type f -iname *.l)
PARSEFILE := $(shell find $(SRC)/$(Q) -type f -iname *.y)
SCANC     := lex.yy.c
SCANOUT   := $(SRC)/$(Q)/$(SCANC)
PARCOUT   := $(patsubst %.y,%.tab.c,$(PARSEFILE))
PARC      := $(patsubst $(SRC)/$(Q)/%,%,$(PARCOUT))
PARHOUT   := $(patsubst %.y,%.tab.h,$(PARSEFILE))
PARH      := $(patsubst $(SRC)/$(Q)/%,%,$(PARHOUT))
PAROUT    := $(PARCOUT) $(PARHOUT)

# .c/.cpp FILES AND .o FILES ###################################
SUBDIR    := $(shell find $(SRC) -type d)
OBJSUBDIR := $(patsubst $(SRC)%,$(OBJ)%,$(SUBDIR))
ALLCS	  := $(SCANOUT) $(PARCOUT)
ALLCPPS	  := $(shell find $(SRC) -type f -iname *.cpp)

# NOT COMPILE CPPS ON question4 DIR WHEN THE QUESTION IS DIFFERENT OF 4
ifneq ($(Q),question4)
ALLCPPS := $(patsubst $(SRC)/question4/%,,$(ALLCPPS))
endif

ALLOBJS   := $(foreach F,$(ALLCS) $(ALLCPPS),$(call TO,$(F)))


$(APP) : $(SCANOUT) $(PAROUT) $(OBJSUBDIR) $(ALLOBJS)
	$(CC) $(ALLOBJS) $(LIBS) -o $@

$(foreach F,$(ALLCS) $(ALLCPPS),$(eval $(call COMPILE,$(CC),$(CFLAGS),$(F),$(INCLUDE),$(call TO,$(F)))))

.PHONY : info clean cleanall libs-clean libs-cleanall

$(SCANOUT):
	lex -C $(SCANFILE)
	mv $(SCANC) $@

$(PAROUT):
	bison -d $(PARSEFILE)
	mv $(PARC) $(PARCOUT)
	mv $(PARH) $(PARHOUT)

$(OBJSUBDIR):
	mkdir -p $@

clean:
	rm $(SCANOUT) $(PARCOUT) $(PARHOUT)
	rm -r $(OBJ)

cleanall:
	rm $(SCANOUT) $(PARCOUT) $(PARHOUT)
	rm -r $(OBJ)
	rm $(APP)

libs:
	$(MAKE) -C $(LIB)

libs-clean:
	$(MAKE) -C $(LIB) clean

libs-cleanall:
	$(MAKE) -C $(LIB) cleanall

info:
	$(info $(SCANFILE))
	$(info $(PARSEFILE))
	$(info $(PARC))
	$(info $(PARCOUT))
	$(info $(PARH))
	$(info $(PARHOUT))
	$(info $(OBJSUBDIR))
	$(info $(ALLCS))
	$(info $(ALLCPPS))
	$(info $(ALLOBJS))
