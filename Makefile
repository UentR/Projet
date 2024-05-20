.SUFFIXES: .o .cpp
OBJDIR = object
INCDIR = include

CXX = g++
CXXFLAGS = -W -O2 -w -std=c++17 -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing -fopenmp -march=native -D_GLIBCXX_PARALLEL -DPARALLEL

vpath %.cpp src
vpath %.hpp include

PROGS=Jeu Screen Movie 

debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)

objects = $(addprefix $(OBJDIR)/, Terrain.o Fourmi.o Coord.o BaseVariables.o )
depend = $(addprefix $(INCDIR)/, Terrain.hpp Fourmi.hpp Coord.hpp BaseVariables.hpp BoucleJeu.hpp)

jeu = $(addprefix $(OBJDIR)/,  BoucleJeu.o)

screen = $(addprefix $(OBJDIR)/, BoucleJeuScreen.o )

all: $(PROGS)

cleanImg:
	rm -rf PPM/*

clean: cleanImg
	rm -rf $(OBJDIR)/*.o $(PROGS) 

Movie: object/MakeMovie.o
	$(CXX) $(CXXFLAGS) $^ -o Movie
	@echo "Compilation Movie terminée"

Jeu: $(dependDebug) $(debug) $(depend) $(objects) $(jeu)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Screen: $(dependDebug) $(debug) $(depend) $(objects) $(screen)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system
	@echo "Compilation screen terminée"

LineNumber:
	@wc -l src/*.cpp include/*.hpp

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@