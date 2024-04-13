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
    
        bool setCoord(int a, int b);
        int getLigne() const;
        int getColonne() const;
};