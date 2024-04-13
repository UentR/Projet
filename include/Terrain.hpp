#include <array>
#include "Fourmi.hpp"
#include <map>

using namespace std;

class Cell {
    public:
        map<int, int> pheromones;
        unsigned int state;                  // 6 premier bits = colonie correspondante / 2 derniers = state 
        vector<Fourmi *> toAnt;                 //                                         / 0 = Nid, 1 = Sucre, 2 = Vide, 3 = Mur
        int sugarAmount;

        bool containsSugar() const;             // Done
        bool containsNest() const;              // Done
        bool containsNest(Colonie c) const;     // 
        bool containsNest(int Idx) const;       // Done
        bool isEmpty() const;                   
        bool containsPheromone(int a) const;    // Done
        bool containsPheromone() const;         // Done
        bool containsAnt() const;               // Done

        void update();

        Cell(int Sugar, int State);

};


class Terrain {
    private:
        const int width;
        const int height;
        
    
    public:
        vector<Cell> Cells;
        int getWidth() const;           // Done
        int getHeight() const;          // Done
        int *toCoord(int Idx) const;    // Done

        void toText() const;
        void toPPM() const;
        void toScreen() const;

        Terrain(int w, int h);          // Done
};