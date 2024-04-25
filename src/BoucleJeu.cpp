#include "BoucleJeu.hpp"

#include <iomanip>      // pour setfill, setw
#include <sstream>      // pour ostringstream
#include <fstream>

#include <cstdlib>
#include <ctime>

using namespace std;

// Constructeurs

Jeu::Jeu(int nbColonies, int W, int H) : nbTours{0}, Width{W}, Height{H} {
    terrain = new Terrain(W, H);
    Colonie *c;
    Cell *cell;
    for (int i=0; i<nbColonies; i++) {
        Coord Coordo = Coord{rand()%W, rand()%H};
        c = new Colonie(terrain, Coordo, i);
        colonies.push_back(c);
        cell = terrain->getCell(c->getPosition());
        if (!(cell->setNest(c))) {
            delete c;
            i--;
        }
    }
}

Jeu::Jeu(int W, int H) : Jeu(2, W, H) {}


// Methodes

void Jeu::nextTurn() {
    nbTours++;
    for (Colonie *c : colonies) { // Quelle colonie joue la première ?
        // c->nextTurn();
    }
    terrain->updateCell();
}




#define SQUARESIZE 30



// Methode print screen

void repeatChar(int Nbr, char c) {
    for (int i=0; i<Nbr; i++) {
        cout << c;
    }
}

void repeatChar(int Nbr, string c) {
    for (int i=0; i<Nbr; i++) {
        cout << c;
    }
}

void Jeu::toText() const {
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
}

// for (auto const& [key, val] : pheromonesSucre)

void Jeu::toPPM() const {
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
            if (Current->containsAnt()) {
                c = Current->getNest();
                r = c->getColor()[0];
                g = c->getColor()[1];
                b = c->getColor()[2];
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
}

int main() {
    srand(time(NULL));
    Jeu j{3, 50, 50};
    j.toPPM();
    return 0;
}