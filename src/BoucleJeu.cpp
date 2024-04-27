#include "BoucleJeu.hpp"

#include <iomanip>      // pour setfill, setw
#include <sstream>      // pour ostringstream
#include <fstream>

#include <cstdlib>
#include <ctime>

using namespace std;

// Constructeurs

Jeu::Jeu(int W, int H, int nbColonies) : nbTours{0}, Width{W}, Height{H} {
    writeToDebugFile("Jeu constructor", INFO_DETAIL);
    terrain = new Terrain(W, H);
    Colonie *c;
    Cell *cell;
    for (int i=0; i<nbColonies; i++) {
        writeToDebugFile("Colonie: " + to_string(i), ALL_LOG);
        Coord Coordo = Coord{rand()%W, rand()%H};
        cell = terrain->getCell(Coordo);
        if (cell->containsNest()) {
            i--;
            continue;
        }
        c = new Colonie(terrain, Coordo, i);
        colonies.push_back(c);
        cell->setNest(c);
    }
    writeToDebugFile("Jeu constructor fin", INFO_DETAIL);
}

Jeu::Jeu(int W, int H) : Jeu(2, W, H) {}


// Methodes

void Jeu::nextTurn() {
    writeToDebugFile("nextTurn", INFO_DETAIL);
    nbTours++;
    for (Colonie *c : colonies) { // Quelle colonie joue la première ?
        c->nextTurn();
    }
    terrain->updateCell();
    writeToDebugFile("nextTurn fin", INFO_DETAIL);
}


void Jeu::BoucleJeu() {
    writeToDebugFile("BoucleJeu", INFO_DETAIL);
    while (nbTours < 3) {
        writeToDebugFile("Tour " + to_string(nbTours), ALL_LOG);
        nextTurn();
        toPPM();
    }
    writeToDebugFile("BoucleJeu fin", INFO_DETAIL);
}


// Methode print screen

void repeatChar(int Nbr, char c) {
    writeToDebugFile("repeatChar char", ALL_LOG);
    for (int i=0; i<Nbr; i++) {
        cout << c;
    }
    writeToDebugFile("repeatChar char fin", ALL_LOG);
}

void repeatChar(int Nbr, string c) {
    writeToDebugFile("repeatChar string", ALL_LOG);
    for (int i=0; i<Nbr; i++) {
        cout << c;
    }
    writeToDebugFile("repeatChar string fin", ALL_LOG);
}

void Jeu::toText() const {
    writeToDebugFile("toText", INFO_DETAIL);
    int LineSize = Width;
    cout << UpRightC;
    repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+TUp);
    cout << Horizontal+Horizontal+Horizontal+UpLeftC;

    int Idx=0;
    Cell *Current;
    for (int y=0; y<Height; y++) {
        cout << endl << Vertical;
        for (int x=0; x<Width; x++) {
            Current = terrain->Cells[Idx];
            if (Current->containsAnt()) {
                cout << " " << Triangle << " ";
            } else if (Current->containsNest()) {
                cout << " " << Carre << " ";
            } else if (Current->containsSugar()) {
                cout << " " << Circle << " ";
            } else if (Current->containsWall()) {
                cout << Full << Full << Full;
            } else {
                cout << "   ";
            }
            cout << Vertical;
            
            Idx++;
        }
        cout << endl;
        cout << TLeft;
        repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+Cross);
        cout << Horizontal+Horizontal+Horizontal+TRight;
    }
    cout << '\r';
    cout << DownRightC;
    repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+TDown);
    cout << Horizontal+Horizontal+Horizontal+DownLeftC << endl;
    writeToDebugFile("toText fin", INFO_DETAIL);
}

// for (auto const& [key, val] : pheromonesSucre)

void Jeu::toPPM() const {
    writeToDebugFile("toPPM", INFO_DETAIL);
    int i,j;
    int r,g,b;
    ostringstream filename;
    // creation d'un nouveau nom de fichier de la forme img347.ppm
    filename << "PPM/img" << setfill('0') << setw(3) << nbTours << ".ppm";
    cout << "Ecriture dans le fichier : " << filename.str() << endl;
    // ouverture du fichier
    ofstream fic(filename.str(), ios::out | ios::trunc);
    // ecriture de l'entete
    fic << "P3" << endl
        << Width*SQUARESIZE << " " << Height*SQUARESIZE << " " << endl
        << 255 << " " << endl;

    // écriture pixel
    ostringstream line;
    Colonie *c;
    Cell *Current;
    map<int, int> Phero;
    for (int y=0; y<Height; y++) {
        // cout << y
        for (int x=0; x<Width; x++) {
            Current = terrain->Cells[terrain->toIdx(x, y)];
            Current = terrain->getCell(Coord{x, y});
            if (Current->containsAnt()) {
                c = Current->getNest();
                // r = c->getColor()[0];
                // g = c->getColor()[1];
                // b = c->getColor()[2];
                r = 150;
                g = 150;
                b = 150;

            } else if (Current->containsNest()) {
                c = Current->getNest();
                r = c->getColor()[0];
                g = c->getColor()[1];
                b = c->getColor()[2];
            } else if (Current->containsSugar()) {
                r = 244;
                g = 223;
                b = 177;
            } else if (Current->containsWall()) {
                r = 0;
                g = 0;
                b = 0;
            } else if (Current->containsPheromone()) {
                Phero = Current->pheromonesSucre;
                int *color = new int[3];
                for (auto const& [key, val] : Phero) {
                    for (int i=0; i<3; i++) {
                        color[i] = val * (key==i);
                    }
                }
                r = color[0];
                g = color[1];
                b = color[2];
            } else {
                // Empty
                r = 255;
                g = 255;
                b = 255;
            }
            for (int Size=0; Size<SQUARESIZE; Size++)
                line << r << " " << g << " " << b << " ";
        }
        for (int Size=0; Size<SQUARESIZE; Size++)
            fic << line.str() << endl;
        line.str("");
    }
    // fermeture du fichier
    fic.close();
    writeToDebugFile("toPPM fin", INFO_DETAIL);
}

int main() {
    flushDebug();
    writeToDebugFile("main", INFO_DETAIL);
    srand(time(NULL));
    Jeu j{50, 50, 3};
    j.toPPM();
    j.BoucleJeu();
    return 0;
}