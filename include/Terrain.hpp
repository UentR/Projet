#ifndef TERRAIN
#define TERRAIN

#ifndef FOURMI
#include "Fourmi.hpp"
#endif

#ifndef COORD
#include "Coord.hpp"
#endif

#include <array>
#include <map>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

#define REMOVEPHEROMONES 5

class Colonie;
class Fourmi;
class Coord;

class Cell {
    private:
        map<int, int> pheromonesSucre;
        // map<int, int> pheromonesNid;
        int height;
        int state;                  // 0 = Nid, 1 = Sucre, 2 = Vide, 3 = Mur
        vector<Fourmi *> toAnt;
        int sugarAmount;
        Colonie *nestAbove;
        Coord coord;

    public:
        bool setPheromonesSucre(int Idx, int value);

        /** @b DONE
         * @brief getter the Sugar Amount object
         * 
         * @return int 
         */
        int getSugarAmount() const;

        /**
         * @brief Get the To Ant object
         * 
         * @return vector<Fourmi *> 
         */
        vector<Fourmi *> getToAnt() const;

        /**
         * @brief Get the State object
         * 
         * @return int 
         */
        int getState() const;

        /**
         * @brief Get the Coord object
         * 
         * @return Coord 
         */
        Coord getCoord() const;

        /**
         * @brief Get the Nest Above object
         * 
         * @return Colonie* 
         */
        Colonie *getNestAbove() const;

        /**
         * @brief Get the Pheromones Sucre object
         * 
         * @return map<int, int> 
         */
        map<int, int> getPheromonesSucre() const;

        /**
         * @brief Enlever une fourmi d'une case
         * @param Fourmi* Fourmi à enlever
         */
        void removeAnt(Fourmi *f);

        /**
         * @brief Ajouter une fourmi à une case
         * @param Fourmi* Fourmi à ajouter
         */
        void addAnt(Fourmi *f);

        /**
         * @brief Set the Nest object
         * 
         * @param Colonie* : Colonie à ajouter
         * @return Réussite de l'opération 
         */
        bool setNest(Colonie *c);

        /** @b DONE
         * @return true si la cellule contient du sucre
         */
        bool containsSugar() const;

        /** @b DONE
         * @return true si la cellule contient un nid
         */
        bool containsNest() const;

        /** @b DONE
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
        Cell(int Sugar, int State, int Height, Coord Coordo);

        /** @b DONE
         * @brief Constructeur de Cell avec variables
         */
        Cell(int Sugar, int State, int Height, Coord Coordo, Colonie *Col);

        /** @b DONE
         * @brief Constructeur de Cell par défaut
         */
        Cell();

};


class Terrain {
    private:
        const int width;
        const int height;
        vector<Cell *> Cells;
    
    public:
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

        /** @b DONE
         * @brief Update les phéromones des cellules
         */
        void updateCell();

        /** @b DONE
         * @return vector<Cell *> Ensemble des cellules
         */
        vector<Cell *> getCells() const;

        /** @b DONE
         * @param Coord Coordonnées de la cellule
         * @return Cell* Cellule associée
         */
        Cell *getCell(Coord c) const;

        /** @b DONE
         * @param int Index de la cellule
         * @return Cell* Cellule associée
         */
        Cell *getCell(int Idx) const;

        /** @b DONE
         * @brief Trouve les voisins d'une cellule
         * @param Coord Coordonnées de la cellule
         * @param int Quantité de sucre à ajouter
         * @return Voisin de la cellule
         */
        vector<Cell *> voisin(Coord c, int Rayon) const;

        /** @b DONE
         * @brief Trouve les voisins d'une cellule avec un état donné
         * @param Coord Coordonnées de la cellule
         * @param int Quantité de sucre à ajouter
         * @return Voisin de la cellule avec un état donné
         */
        vector<Cell *> voisinState(Coord c, int Rayon, int State) const;

        /** @b DONE
         * @brief Constructeur de Terrain avec variables
         */
        Terrain(int w, int h);
};

ostream &operator<<(ostream &out, Cell c);

ostream &operator<<(ostream &out, Terrain t);



#endif