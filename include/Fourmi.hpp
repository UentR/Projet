#define FOURMI

#ifndef TERRAIN
#include "Terrain.hpp"
#endif

#include <array>
#include "Debug.hpp"
#include <vector>
#include "Coord.hpp"
#include "BaseVariables.hpp"
#include <set>

using namespace std;

class Colonie;
class Terrain;

class Fourmi {
    public:
        float vie;
        int type;
        int vision;
        Coord position;
        float quantiteSucre;
        float capaciteSucre;
        float forceAttaque;
        bool isAttacked;
        int colonieIdx;
        Colonie *colonie;
        Terrain *terrain;
        
        /** @b DONE
         * @brief Check si la fourmi est au nid
         */
        bool auNid() const;

        /** @b NOT-DONE
         * @brief Check si la fourmi est sur du sucre
         */
        bool auSucre() const;

        /** @b DONE
         * @brief Check si la fourmi est proche d'un état donné
         * @param int état à vérifier
         * @return Coord : coordonnées du premier état trouvé
         */
        Coord procheState(int State) const;

        /** @b DONE
         * @brief Check si la fourmi est proche d'un phéromone donné
         * @param int Index du phéromone à vérifier
         * @return vector<Coord> : coordonnées des endroits avec dits phéromones
         */
        vector<Coord> pheromonesProches(int Idx) const;

        /** @b DONE
         * @brief Attaque une fourmi
         * @param Fourmi* Fourmi à attaquer
         * @return float : Vie de la fourmi attaquée
         */
        float attaquer(Fourmi *f);

        /** @b DONE
         * @brief Ramasse du sucre
         * @param Terrain Terrain sur lequel se trouve la fourmi
         * @param Coord Coordonnées de la case à ramasser
         * @return int : Quantité de sucre ramassée
         */
        int ramasserSucre(Coord c);

        /** @b DONE
         * @brief Dépose du sucre dans le nid
         * @param Terrain Terrain sur lequel se trouve la fourmi
         * @param Coord Coordonnées de la case à déposer
         * @return bool : Si le sucre a été déposé
         */
        bool deposerSucre(Coord c);

        /** @b DONE
         * @brief Déplace la fourmi
         * @param Terrai Terrain sur lequel se trouve la fourmi
         * @param Coord Coordonnées de la case où se déplacer
         * @return bool : Si la fourmi a été déplacée
         */
        bool deplacer(Coord c);

        /** @b DONE
         * @brief Met des phéromones sur la case
         * @param Terrain Terrain sur lequel se trouve la fourmi
         */
        void mettrePheromone() const;

        /** @b DONE
         * @brief Check les fourmis aux alentours
         * @return vector<Fourmi *> : Fourmis aux alentours
         */
        vector<Fourmi *> fourmiProche() const;

        /** @b DONE
         * @brief Permet à une fourmi de choisir une action de base
         */
        void choixAction();

        /** @b NOT-DONE
         * @brief Soigne la fourmi
         */
        void soigner();

        /** @b DONE
         * @brief Constructeur des fourmis avec variables
         */
        Fourmi(Coord c, int Idx, Colonie *col, Terrain *t);

        /** @b DONE
         * @brief Constructeur des fourmis par défaut
         */
        Fourmi();

};

class Guerriere : public Fourmi {
    public:
        /** @b DONE
         * @brief Choix de l'action de la guerrière
         */
        void actionGuerriere();

        /** @b DONE
         * @brief Constructeur de la guerrière
         */
        Guerriere(Coord c, int Idx, Colonie *col, Terrain *t);
};

class Ouvriere : public Fourmi {
    public:
        /** @b ~DONE
         * @brief Fuit
         */
        void fuir();

        /** @b DONE
         * @brief Choix de l'action de l'ouvrière
         */
        void actionOuvriere();

        /** @b DONE
         * @brief Constructeur de l'ouvrière
         */
        Ouvriere(Coord c, int Idx, Colonie *col, Terrain *t);
};

class Reproductrice : public Ouvriere {
    public:
        /** @b DONE
         * @brief Check si la colonie peut se reproduire
         */
        bool checkReproduction();

        /** @b NOT-DONE
         * @brief La fourmi se rapproche du nid
         */
        void rentrerNid(); 
        
        /** @b DONE
         * @brief Choix de l'action de la reproductrice
         */
        void actionReproductrice();
        
        /** @b DONE
         * @brief Constructeur de la reproductrice
         */
        Reproductrice(Coord c, int Idx, Colonie *col, Terrain *t);
};


// ind : 0 = ouvriere ; 1 = guerriere  ; 2 reproductrice

class Colonie {
    private:
        int nbFourmis;
        array<int, 3> typeFourmis;
        set<Fourmi *> reproductriceEnAttente;
        Coord position;
        Terrain *terrain;

    public:
        
        int Idx;
        int quantiteSucre;
        vector<Fourmi *> fourmis;

        Terrain *getTerrain();

        /** @b DONE
         * @brief Ajoute une fourmi à la liste des fourmis pretes à se reproduire
         * @param Fourmi* Fourmi à ajouter
         */
        void pretReproduction(Fourmi *f);

        /** @b DONE
         * @brief Check si la colonie peut se reproduire
         */
        bool CheckReproduction();

        /** @b DONE
         * @brief Supprime une fourmie de la colonie
         */
        void deleteFourmi(Fourmi *f);

        /** @b DONE
         * @return int : Nombre de fourmis dans la colonie
         */
        int getNbFourmis() const;

        /** @b DONE
         * @return int : Index de la colonie
         */
        int getIdx();

        /** @b DONE
         * @return Coord : Position de la colonie
         */
        Coord getPosition() const;

        /** @b DONE
         * @brief Produit une fourmi du premier type ayant un ratio trop faible
         * @return int : Le type de fourmi produite
         */
        int produireFourmis();

        /** @b DONE
         * @brief Constructeur de la colonie
         */
        Colonie(Terrain *t, Coord c);
};

/** @b DONE
 * @brief Surcharge de l'opérateur << pour les fourmis
 */
ostream &operator<<(ostream &out, Fourmi *f);

/** @b DONE
 * @brief Surcharge de l'opérateur << pour les colonies
 */
ostream &operator<<(ostream &out, Colonie c);