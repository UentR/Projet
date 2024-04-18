#define TERRAIN

#ifndef FOURMI
#include "Fourmi.hpp"
#endif

#include <array>
#include <map>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>

using namespace std;


#define HWall 0.1
#define HEmpty 0
#define HSugar 0.8
#define HNest 1

#define REMOVEPHEROMONES 5

class Colonie;
class Fourmi;
class Coord;

class Cell {
    public:
        map<int, int> pheromones;
        int height;
        unsigned short int state;               // 6 premier bits = colonie correspondante / 2 derniers = state 
        vector<Fourmi *> toAnt;                 //                                         / 0 = Nid, 1 = Sucre, 2 = Vide, 3 = Mur
        int sugarAmount;
        Colonie *nestAbove;

        unsigned short int getState() const;

        /** @b DONE
         * @return true si la cellule contient du sucre
         */
        bool containsSugar() const;

        /** @b DONE
         * @return true si la cellule contient un nid
         */
        bool containsNest() const;

        /** @b NOT-DONE
         * @param Colonie Colonie à vérifier
         * @return true si la cellule contient un nid de la colonie donnée
         */
        bool containsNest(Colonie *c) const;

        /** @b DONE
         * @param int Identifiant de la colonie
         * @return true si la cellule contient un nid de la colonie du même identifiant
         */
        bool containsNest(int Idx) const;

        /** @b DONE
         * @return true si la cellule contient un mur
         */
        bool containsWall() const;

        /** @b DONE
         * @return true si la cellule est vide
         */
        bool isEmpty() const;

        /** @b DONE
         * @param unsigned_int Identifiant du phéromone
         * @return true si la cellule contient le phéromone du même identifiant
         */
        bool containsPheromone(unsigned int a) const;

        /** @b DONE
         * @param int Identifiant du phéromone
         * @return true si la cellule contient un phéromone différent de l'identifiant
         */
        bool containsPheromone(int a) const;

        /** @b DONE
         * @return true si la cellule contient un phéromone
         */
        bool containsPheromone() const;

        /** @b DONE
         * @return true si la cellule contient une fourmi
         */
        bool containsAnt() const;

        /** @b DONE
         * @brief Update les valeurs des phéromones
         */
        void update();

        /** @b DONE
         * @brief Retire du sucre à la cellule
         * @param int Quantité de sucre à retirer
         * @return Quantité de sucre retirée
         */
        int removeSugar(int amount);

        /** @b DONE
         * @brief Constructeur de Cell avec variables
         */
        Cell(int Sugar, int State, int Height);

        /** @b DONE
         * @brief Constructeur de Cell avec variables
         */
        Cell(int Sugar, int State, Colonie *Col);

        /** @b DONE
         * @brief Constructeur de Cell par défaut
         */
        Cell();

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
        vector<Cell *> Cells;

        /** @b DONE
         * @param int Coordonnée X
         * @param int Coordonnée Y
         * @return Y*Width + X
        */
        int toIdx(int x, int y) const;

        /** @b DONE
         * @return Largeur du terrain
         */
        int getWidth() const;

        /** @b DONE
         * @return Hauteur du terrain
         */
        int getHeight() const;

        /** @b DONE
         * @param int Index de la cellule
         * @return Coordonnées de la cellule associée
         */
        int *toCoord(int Idx) const;


        Cell *getCell(Coord c) const;

        /** @b DONE
         * @brief Affiche le terrain en texte
         */
        void toText() const;

        /** @b NOT-DONE
         * @brief Affiche le terrain en PPM
         */
        void toPPM() const;

        /** @b NOT-DONE
         * @brief Affiche le terrain via un écran
         */
        void toScreen() const;

        /** @b DONE
         * @brief Constructeur de Terrain avec variables
         */
        Terrain(int w, int h);
};
