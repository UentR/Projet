#include <array>
#include "Debug.hpp"
#include <vector>
#include "Coord.hpp"
#include "BaseVariables.hpp"

using namespace std;

class Fourmi {
    public:
        float vie;
        int type;
        Coord position;
        float quantiteSucre;
        float capaciteSucre;
        float forceAttaque;
        bool isAttacked;
        int colonie;
        
        float attaquer(Coord c);
        float ramasserSucre(Coord c);
        bool deposerSucre(Coord c);
        bool deplacer(Coord c);
        bool mettrePheromone() const;
        void choixAction();
        void soigner();
        void fourmiProche() const;

        Fourmi(Coord c, int Idx);
        Fourmi();

};

class Guerriere : public Fourmi {
    private:        
        void suivrePheromone();
    
    public:
        void actionGuerriere();
    
        Guerriere(Coord c, int Idx);                            // Done
};

class Ouvriere : public Fourmi {
    private:
        void suivrePheromone();
    
    public:
        void fuir();
        void actionOuvriere();

        Ouvriere(Coord c, int Idx);                             // Done
};

class Reproductrice : public Fourmi {
    private:
        void suivrePheromone();
        
    public:
        bool checkReproduction();
        void rentrerNid();
        void attendrePartenaire();
        void Reproduire();
        
        void actionRepoductrice();
        
        Reproductrice(Coord c, int Idx);                        // Done
};


// ind : 0 = ouvriere ; 1 = guerriere  ; 2 reproductrice

class Colonie {
    private:
        
        int nbFourmis;
        array<int, 3> typeFourmis;
        int reproducticeEnAttente;
        Coord position;

    public:
        int Idx;
        int quantiteSucre;
        vector<Fourmi> fourmis;

        int getNbFourmis() const;                               // Done
        int getIdx();                                           // Done
        Coord getPosition() const;                              // Done
        int produireFourmis();                                  // Done

        Colonie(Coord c);                                       // Done
};

ostream &operator<<(ostream &out, Fourmi f);
ostream &operator<<(ostream &out, Colonie c);