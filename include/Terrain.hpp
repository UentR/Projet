#include <array>

#include <map>
#include <cstdlib>
#include <ctime>



using namespace std;


#define HWall 0.1
#define HEmpty 0
#define HSugar 0.8
#define HNest 1

#define REMOVEPHEROMONES 5


class Cell {
    public:
        map<int, int> pheromones;
        int height;
        unsigned short int state;               // 6 premier bits = colonie correspondante / 2 derniers = state 
        vector<Fourmi *> toAnt;                 //                                         / 0 = Nid, 1 = Sucre, 2 = Vide, 3 = Mur
        int sugarAmount;
        Colonie *nestAbove;

        bool containsSugar() const;             // Done
        bool containsNest() const;              // Done
        bool containsNest(Colonie c) const;     // 
        bool containsNest(int Idx) const;       // Done
        bool containsWall() const;              // Done
        bool isEmpty() const;                   //
        bool containsPheromone(unsigned int a) const; // Done
        bool containsPheromone(int a) const;    // Done
        bool containsPheromone() const;         // Done
        bool containsAnt() const;               // Done

        void update();                          // Done
        int removeSugar(int amount);            // Done

        Cell(int Sugar, int State, int Height); // Done
        Cell();                                 // Done

};


class Terrain {
    private:
        const int width;
        const int height;

        const string UpLeftC = "\U00002513";
        const string UpRightC = "\U0000250F";
        const string DownLeftC = "\U0000251B";
        const string DownRightC = "\U00002517";

        const string Vertical = "\U00002503";
        const string Horizontal = "\U00002501";

        const string Cross = "\U0000254B";

        const string TLeft = "\U00002523";
        const string TUp = "\U00002533";
        const string TRight = "\U0000252B";
        const string TDown = "\U0000253B";

        const string Triangle = "\U000025B2";
        const string Carre = "\U000025A0";
        const string Circle = "\U000025CF";

        const string Full = "\U00002588";
        
    
    public:
        vector<Cell> Cells;
        int getWidth() const;           // Done
        int getHeight() const;          // Done
        int *toCoord(int Idx) const;    // Done

        void toText() const;            // Done
        void toPPM() const;
        void toScreen() const;

        Terrain(int w, int h);          // Done
};