.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -W -O3 -w -std=c++17 -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing
OBJDIR = object
INCDIR = include
vpath %.cpp src
vpath %.hpp include

PROGS=Jeu
debug = $(addprefix $(OBJDIR)/, Debug.o)
dependDebug = $(addprefix $(INCDIR)/, Debug.hpp)

objectCoord = $(addprefix $(OBJDIR)/, Coord.o )
includeCoord = $(addprefix $(INCDIR)/, Coord.hpp )

objectTerrain = $(addprefix $(OBJDIR)/, Terrain.o )
includeTerrain = $(addprefix $(INCDIR)/, Terrain.hpp )

objects = $(addprefix $(OBJDIR)/, Terrain.o Fourmi.o Coord.o BaseVariables.o BoucleJeu.o)
depend = $(addprefix $(INCDIR)/, Terrain.hpp Fourmi.hpp Coord.hpp BaseVariables.hpp BoucleJeu.hpp)


jeu = $(addprefix $(OBJDIR)/,  )
jeudepend = $(addprefix $(INCDIR)/, )

demo = $(addprefix $(OBJDIR)/, )
demodepend = $(addprefix $(INCDIR)/, )

all: $(PROGS)

clean:
	rm -rf $(OBJDIR)/*.o $(PROGS) Movie PPM/*

Coord: $(includeCoord) $(objectCoord)
	@$(CXX) $(CXXFLAGS) $^ -o Coord
	@echo "Compilation Coord terminée"

Movie:
	@$(CXX) $(CXXFLAGS) -std=c++17 -DPARELLEL -O2 src/MakeMovie.cpp -o Movie
	@echo "Compilation Movie terminée"

Terrain: $(includeCoord) $(objectCoord) $(dependDebug) $(debug) $(depend) $(object) $(includeTerrain) $(objectTerrain)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation terrain terminée"

Fourmi: $(dependDebug) $(debug) $(includeTerrain) $(objectTerrain) $(depend) $(objects) $(includeCoord) $(objectCoord) 
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Jeu: $(dependDebug) $(debug) $(depend) $(objects)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

LineNumber:
	@wc -l src/*.cpp include/*.hpp

Demo:  $(debug) $(demo)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation demo terminée"

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@