.SUFFIXES: .o .cpp
OBJDIR = object
INCDIR = include

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -W -O2 -w -std=c++17 -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing -fopenmp -march=native -D_GLIBCXX_PARALLEL -DPARALLEL
CFLAGS = -W -O3 -w -std=c++17 -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing

vpath %.cpp src
vpath %.hpp include

PROGS=Jeu Screen Movie 

debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)

objectCoord = $(addprefix $(OBJDIR)/, Coord.o )
includeCoord = $(addprefix $(INCDIR)/, Coord.hpp )

objectTerrain = $(addprefix $(OBJDIR)/, Terrain.o )
includeTerrain = $(addprefix $(INCDIR)/, Terrain.hpp )

objects = $(addprefix $(OBJDIR)/, Terrain.o Fourmi.o Coord.o BaseVariables.o )
depend = $(addprefix $(INCDIR)/, Terrain.hpp Fourmi.hpp Coord.hpp BaseVariables.hpp BoucleJeu.hpp)


jeu = $(addprefix $(OBJDIR)/,  BoucleJeu.o)
jeudepend = $(addprefix $(INCDIR)/, )

demo = $(addprefix $(OBJDIR)/, )
demodepend = $(addprefix $(INCDIR)/, )

screen = $(addprefix $(OBJDIR)/, BoucleJeuScreen.o )

all: $(PROGS)

cleanImg:
	rm -rf PPM/*

clean: cleanImg
	rm -rf $(OBJDIR)/*.o $(PROGS) Movie 

Coord: $(includeCoord) $(objectCoord)
	@$(CXX) $(CXXFLAGS) $^ -o Coord
	@echo "Compilation Coord terminée"

Movie: object/MakeMovie.o
	$(CXX) $(CXXFLAGS) $^ -o Movie
	@echo "Compilation Movie terminée"

Terrain: $(includeCoord) $(objectCoord) $(dependDebug) $(debug) $(depend) $(object) $(includeTerrain) $(objectTerrain)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation terrain terminée"

Fourmi: $(dependDebug) $(debug) $(includeTerrain) $(objectTerrain) $(depend) $(objects) $(includeCoord) $(objectCoord) 
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Jeu: $(dependDebug) $(debug) $(depend) $(objects) $(jeu)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Screen: $(dependDebug) $(debug) $(depend) $(objects) $(screen)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system
	@echo "Compilation screen terminée"

LineNumber:
	@wc -l src/*.cpp include/*.hpp

compile:
	g++ -IsrcSFML/include -c TestSFML.cpp -o TestSFML.o

link:
	g++ *.o -o TestSFML -LsrcSFML/lib -lsfml-graphics -lsfml-window -lsfml-system


$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@