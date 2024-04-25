#ifndef COORD
#define COORD

#include <iostream>
#include <array>
#include <stdexcept>
#include <vector>


using namespace std;

const int TAILLEGRILLE = 1000; 

class Coord{
    private:
        int ind_col;
        int ind_lig;

    public:
        /** @b DONE
         * @brief Constructeurs de Coord avec variables
         */
        Coord(int a, int b);

        /** @b DONE
         * @brief Constructeur de Coord par défaut
         */
        Coord();

        /** @b DONE
         * @brief Setter des coordonnees
         */
        bool setCoord(int a, int b);

        /** @b DONE
         * @brief Getter de la ligne
         */
        int getLigne() const;

        /** @b DONE
         * @brief Getter de la colonne
         */
        int getColonne() const;

        vector<Coord> voisin(int Rayon) const;
};

/** @b DONE
 * @brief Surcharge de l'operateur <<
 */
ostream &operator<<(ostream &out, Coord c);

/** @b DONE
 * @brief Surcharge de l'operateur ==
 */
bool operator==(Coord c1, Coord c2);

/** @b DONE
 * @brief Surcharge de l'operateur !=
 */
bool operator!=(Coord c1, Coord c2);

struct EnsembleCoord{
    private:
        // vector<Coord>(taille) Grille;
        vector<Coord> Grille;
        int position(Coord objet) const;
    public:
        vector<Coord> getGrille() const;

        EnsembleCoord(vector<Coord>);
        bool contient(Coord objet) const;
        void ajoute(Coord objet);
        void supprime(Coord objet);
        bool estVide() const;
        int taille() const;
        Coord ieme(int n) const;
};

ostream &operator<<(ostream &out, EnsembleCoord);

#endif