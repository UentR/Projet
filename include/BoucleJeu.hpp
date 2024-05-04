#ifndef __BOUCLEJEU_HPP
#define __BOUCLEJEU_HPP

#include "BaseVariables.hpp"
#include "Fourmi.hpp"
#include "Terrain.hpp"
#include "Coord.hpp"
#include <SFML/Graphics.hpp>

// Variables globales
char* envNbTourLevel = getenv("NbTour");
bool EmptyNbTourLevel = envNbTourLevel == NULL;
const int NbTour = (EmptyNbTourLevel) ? 5 : atoi(envNbTourLevel);


class Jeu {
    private:
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
        Terrain *terrain;
        vector<Colonie *> colonies;
        int nbTours;
        int Width, Height;

        Jeu(int W, int H,int nbColonies);
        Jeu(int W, int H);

        void BoucleJeu();
        void nextTurn();

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
        void BoucleJeuScreen();
};

#endif