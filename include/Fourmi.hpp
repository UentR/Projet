#include <array>
#include "Debug.hpp"
#include <vector>
#include "Coord.hpp"
#include "BaseVariables.hpp"
#include "Terrain.hpp"
#include <set>


using namespace std;

class Fourmi {
    public:
        float vie;
        int type;
        int vision;
        Coord position;
        float quantiteSucre;
        float capaciteSucre;
        float forceAttaque;
        bool isAttacked;
        int colonieIdx;
        Colonie *colonie;
        
        bool auNid() const;                                             // Done
        bool auSucre() const;                                           // 
        Coord procheState(int State) const;                             // Done
        vector<Coord> pheromonesProches(int Idx) const;     // Done

        float attaquer(Fourmi *);                           // Done
        int ramasserSucre(Terrain t, Coord c);              // Done
        bool deposerSucre(Terrain t, Coord c);              // Done
        bool deplacer(Terrain t, Coord c);                  // Done
        void mettrePheromone(Terrain t) const;              // Done
        vector<Fourmi *> fourmiProche() const;              // Done
        void choixAction(Terrain t);                        // Done
        void soigner();                                     // 

        Fourmi(Coord c, int Idx, Colonie *c);           // Done
        Fourmi();                                       // Done

};

class Guerriere : public Fourmi {
    public:
        void actionGuerriere(Terrain t);        // Done
    
        Guerriere(Coord c, int Idx);            // Done
};

class Ouvriere : public Fourmi {
    public:
        void fuir();                            // ~Done
        void actionOuvriere(Terrain t);         // Done

        Ouvriere(Coord c, int Idx);             // Done
};

class Reproductrice : public Ouvriere {
    public:
        bool checkReproduction();               // Done
        void rentrerNid();                      // 
        
        void actionRepoductrice(Terrain t);     // 
        
        Reproductrice(Coord c, int Idx);        // Done
};


// ind : 0 = ouvriere ; 1 = guerriere  ; 2 reproductrice

class Colonie {
    private:
        int nbFourmis;
        array<int, 3> typeFourmis;
        set<Fourmi *f> reproducticeEnAttente;
        Coord position;

    public:

        int Idx;
        int quantiteSucre;
        vector<Fourmi> fourmis;

        void pretReproduction(Fourmi *);                        // Done
        bool CheckReproduction();                               // Done
        void deleteFourmi(Fourmi f);                            // Done
        int getNbFourmis() const;                               // Done
        int getIdx();                                           // Done
        Coord getPosition() const;                              // Done
        int produireFourmis();                                  // Done

        Colonie(Coord c);                                       // Done
};

ostream &operator<<(ostream &out, Fourmi f);
ostream &operator<<(ostream &out, Colonie c);