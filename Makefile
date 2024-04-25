.SUFFIXES: .o .cpp

CXX = g++
# CXXFLAGS = -W -Wall -std=c++11 -O2 -g -Wno-sign-compare -Wno-narrowing
CXXFLAGS = -W -O2 -w -std=c++17 -Wall -Iinclude -g -Wno-sign-compare -Wno-narrowing
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
	rm -rf $(OBJDIR)/*.o $(PROGS)

Coord: $(includeCoord) $(objectCoord)
	@$(CXX) $(CXXFLAGS) $^ -o Coord
	@echo "Compilation Coord terminée"


Terrain: $(includeCoord) $(objectCoord) $(dependDebug) $(debug) $(depend) $(object) $(includeTerrain) $(objectTerrain)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation terrain terminée"

Fourmi: $(dependDebug) $(debug) $(includeTerrain) $(objectTerrain) $(depend) $(objects) $(includeCoord) $(objectCoord) 
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"

Jeu: $(dependDebug) $(debug) $(depend) $(objects)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation jeu terminée"


Demo:  $(debug) $(demo)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Compilation demo terminée"

$(OBJDIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@