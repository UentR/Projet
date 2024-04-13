.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -O2 -w -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing
OBJDIR = object
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Fourmi
debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)

objects = $(addprefix $(OBJDIR)/, Fourmi.o )
depend = $(addprefix $(INCDIR)/, Fourmi.hpp )

jeu = $(addprefix $(OBJDIR)/,  )
jeudepend = $(addprefix $(INCDIR)/, )

demo = $(addprefix $(OBJDIR)/, )
demodepend = $(addprefix $(INCDIR)/, )

all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS)

Coord: Coord.hpp Coord.o
	@$(CXX) $(CXXFLAGS) Coord.o -o Coord
	@echo "Compilation Coord terminée"

Fourmi: $(objects) $(debug) $(jeu)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Demo:  $(debug) $(demo)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation demo terminée"

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@