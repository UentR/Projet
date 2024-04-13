#include <array>
#include "Debug.hpp"
#include <vector>
#include "Coord.hpp"

using namespace std;

#define VieO 10
#define VieG 15
#define VieR 10

#define AttaqueO 3
#define AttaqueG 6
#define AttaqueR 2

#define CapaciteSO 5
#define CapaciteSG 10
#define CapaciteSR 5

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


#define PourcentO 0.75
#define PourcentG 0.15
#define PourcentR 0.10

// define array with all the pourcentages
array<float, 3> PourcentF = {PourcentO, PourcentG, PourcentR};

#define NBO 15
#define NBG 3
#define NBR 2

array<int, 3> NbF = {NBO, NBG, NBR};

// ind : 0 = ouvriere ; 1 = guerriere  ; 2 reproductrice

class Colonie {
    private:
        int Idx;
        int nbFourmis;
        array<int, 3> typeFourmis;
        int reproducticeEnAttente;
        Coord position;

    public:
        int quantiteSucre;
        vector<Fourmi> fourmis;

        int getNbFourmis() const;                               // Done
        int getIdx() const;                                     // Done
        Coord getPosition() const;                              // Done
        int produireFourmis();                                  // Done

        Colonie(Coord c);                                       // Done
};

ostream &operator<<(ostream &out, Fourmi f);
ostream &operator<<(ostream &out, Colonie c);