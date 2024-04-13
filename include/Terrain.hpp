#include <array>
#include "Fourmi.hpp"
#include <map>
#include <cstdlib>
#include <ctime>


using namespace std;


#define HWall 0.1
#define HEmpty 0
#define HSugar 0.8
#define HNest 1



class Cell {
    public:
        map<int, int> pheromones;
        unsigned short int state;                     // 6 premier bits = colonie correspondante / 2 derniers = state 
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

        const string UpLeftC = "\U00002510";
        const string UpRightC = "\U0000250C";
        const string DownLeftC = "\U00002518";
        const string DownRightC = "\U00002514";

        const string Vertical = "\U00002502";
        const string Horizontal = "\U00002015";

        const string Cross = "\U0000253C";

        const string TLeft = "\U0000251C";
        const string TUp = "\U0000252C";
        const string TRight = "\U00002524";
        const string TDown = "\U00002534";

        const string Full = "\U00002588";
        
    
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