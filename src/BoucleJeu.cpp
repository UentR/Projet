#include "BoucleJeu.hpp"

#include <iomanip>      // pour setfill, setw
#include <sstream>      // pour ostringstream
#include <fstream>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <chrono>

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
    writeToDebugFile("nextTurn JEU", INFO_DETAIL);
    nbTours++;
    for (Colonie *c : colonies) { // Quelle colonie joue la première ?
        c->nextTurn();
    }
    terrain->updateCell();
    writeToDebugFile("nextTurn fin JEU", INFO_DETAIL);
}


void Jeu::BoucleJeu(int type) {
    writeToDebugFile("BoucleJeu", INFO_DETAIL);
    void (Jeu::*funcRepr)() const;
    auto start = chrono::high_resolution_clock::now();
    if (type == 1) {
        std::cout << "Simulation en mode texte" << std::endl;
        toText();
        start = chrono::high_resolution_clock::now();
        funcRepr = &Jeu::toText;
    } else {
        std::cout << "Simulation en mode image" << std::endl;
        toPPM();
        funcRepr = &Jeu::toPPM;
    }

    
    while (nbTours < *VARIABLES["NbTour"]) {
        std::cout << "Tour " << nbTours << std::endl;
        writeToDebugFile("Tour " + to_string(nbTours), ALL_LOG);
        nextTurn();
        while ((type==1) && (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < 1000./(*VARIABLES["FPS"]))) {
            // Wait
            usleep(1000);
        }
        (this->*funcRepr)();
        start = chrono::high_resolution_clock::now();
    }
    writeToDebugFile("BoucleJeu fin", INFO_DETAIL);
}


// Methode print screen

void repeatChar(int Nbr, char c) {
    writeToDebugFile("repeatChar char", ALL_LOG);
    for (int i=0; i<Nbr; i++) {
        std::cout << c;
    }
    writeToDebugFile("repeatChar char fin", ALL_LOG);
}

void repeatChar(int Nbr, string c) {
    writeToDebugFile("repeatChar string", ALL_LOG);
    for (int i=0; i<Nbr; i++) {
        std::cout << c;
    }
    writeToDebugFile("repeatChar string fin", ALL_LOG);
}

void Jeu::toText() const {
    writeToDebugFile("toText", INFO_DETAIL);
    // Clear screen
    system("clear");
    int LineSize = Width;
    std::cout << UpRightC;
    repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+TUp);
    std::cout << Horizontal+Horizontal+Horizontal+UpLeftC;

    int Idx=0;
    Cell *Current;
    for (int y=0; y<Height; y++) {
        std::cout << std::endl << Vertical;
        for (int x=0; x<Width; x++) {
            Current = terrain->getCell(Idx);
            if (Current->containsAnt()) {
                std::cout << " " << Triangle << " ";
            } else if (Current->containsNest()) {
                std::cout << " " << Carre << " ";
            } else if (Current->containsSugar()) {
                std::cout << " " << Circle << " ";
            } else if (Current->containsWall()) {
                std::cout << Full << Full << Full;
            } else {
                std::cout << "   ";
            }
            std::cout << Vertical;
            
            Idx++;
        }
        std::cout << std::endl;
        std::cout << TLeft;
        repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+Cross);
        std::cout << Horizontal+Horizontal+Horizontal+TRight;
    }
    std::cout << '\r';
    std::cout << DownRightC;
    repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+TDown);
    std::cout << Horizontal+Horizontal+Horizontal+DownLeftC << std::endl;
    writeToDebugFile("toText fin", INFO_DETAIL);
}

void Jeu::toPPM() const {
    writeToDebugFile("toPPM", WARNING);
    int i,j;
    int r,g,b;
    ostringstream filename;
    // creation d'un nouveau nom de fichier de la forme img347.ppm
    filename << "PPM/img" << setfill('0') << setw(3) << nbTours << ".ppm";
    std::cout << "Ecriture dans le fichier : " << filename.str() << std::endl;
    // ouverture du fichier
    ofstream fic(filename.str(), ios::out | ios::trunc);
    // ecriture de l'entete
    fic << "P3" << std::endl
        << Width*(*VARIABLES["SQUARESIZE"]) << " " << Height*(*VARIABLES["SQUARESIZE"]) << " " << std::endl
        << 255*2 << " " << std::endl;

    // écriture pixel
    ostringstream line;
    ostringstream All;
    Colonie *c;
    Cell *Current;
    map<int, int> Phero;
    Fourmi *ant;
    for (int y=0; y<Height; y++) {
        // std::cout << y
        for (int x=0; x<Width; x++) {
            Current = terrain->getCell(Coord{x, y});
            if (Current->containsAnt()) {
                writeToDebugFile("Ant", INFO_DETAIL);
                // Get first ant that was on the cell
                ant = Current->getToAnt()[0];
                c = ant->getColonie();
                int *Color = c->getColor();
                r = Color[0]*1.5;
                g = Color[1]*1.5;
                b = Color[2]*1.5;
            } else if (Current->containsNest()) {
                writeToDebugFile("Nest", INFO_DETAIL);
                c = Current->getNestAbove();
                int *Color = c->getColor();
                r = Color[0]*2;
                g = Color[1]*2;
                b = Color[2]*2;
            } else if (Current->containsSugar()) {
                writeToDebugFile("Sugar", INFO_DETAIL);
                r = 244*2;
                g = 223*2;
                b = 177*2;
            } else if (Current->containsWall()) {
                writeToDebugFile("Wall", INFO_DETAIL);
                r = 40*2;
                g = 40*2;
                b = 40*2;
            } else if (Current->containsPheromone()) {
                writeToDebugFile("Pheromone", INFO_DETAIL);
                Phero = Current->getPheromonesSucre();
                int *color = new int[3]{0, 0, 0};
                for (auto const& [key, val] : Phero) {
                    for (int i=0; i<3; i++) {
                        if (key==i) {
                            color[i] = val;
                        }
                    }
                }
                r = color[0];
                g = color[1];
                b = color[2];
            } else {
                writeToDebugFile("Empty", INFO_DETAIL);
                // Empty
                r = 0;
                g = 0;
                b = 0;
            }
            for (int Size=0; Size<*VARIABLES["SQUARESIZE"]; Size++) {
                writeToDebugFile("RGB:" + to_string(r) + " " + to_string(g) + " " + to_string(b), ALL_LOG);
                line << r << " " << g << " " << b << " ";
            }
        }
        writeToDebugFile("line", INFO_DETAIL);
        for (int Size=0; Size<*VARIABLES["SQUARESIZE"]; Size++)
            All << line.str() << std::endl;
        line.str("");
    }
    // fermeture du fichier
    fic << All.str();
    fic.close();
    writeToDebugFile("toPPM fin", WARNING);
}

int main() {
    SetupVar();
    flushDebug();
    writeToDebugFile("main", INFO_DETAIL);
    srand(time(NULL));
    Jeu j{*VARIABLES["TAILLEGRILLE"], *VARIABLES["TAILLEGRILLE"], *VARIABLES["NB_COLONIES"]};
    
    std::cout << "Initialisation du jeu" << std::endl;
    std::cout << "Type de simulation :" << std::endl;
    int type;
    cin >> type;
    j.BoucleJeu(type);

    writeToDebugFile("main fin", ERROR);
    return 0;
}