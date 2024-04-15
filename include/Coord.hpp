#include <iostream>
#include <array>
#include <stdexcept>

using namespace std;

const int TAILLEGRILLE = 100; 

class Coord{
    private:
        int ind_col;
        int ind_lig;

    public:
        Coord(int a, int b);
        Coord();
    
        bool setCoord(int a, int b);
        int getLigne() const;
        int getColonne() const;
};

ostream &operator<<(ostream &out, Coord c);

bool operator==(Coord c1, Coord c2);

bool operator!=(Coord c1, Coord c2);

struct EnsembleCoord{
    private:
        int taille;
        vector<Coord>(taille) Grille;
        EnsembleCoord position(Coord objet);
    public:
        EnsembleCoord(vector<Coord>);
};

ostream &operator<<(ostream &out, EnsembleCoord);
