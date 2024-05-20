#include "BoucleJeu.hpp"

#include <cstdlib>
#include <time.h>
#include <chrono>

using namespace std;

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

void Jeu::nextTurn() {
    writeToDebugFile("nextTurn JEU", INFO_DETAIL);
    nbTours++;
    for (Colonie *c : colonies) {
        c->nextTurn();
    }
    terrain->updateCell();
    writeToDebugFile("nextTurn fin JEU", INFO_DETAIL);
}

void Jeu::BoucleJeuScreen() {
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Projet Fourmis", sf::Style::Titlebar | sf::Style::Close);
    int Size = 1000/(*VARIABLES["TAILLEGRILLE"]);

    // window.setFramerateLimit(60);
    window.setPosition(sf::Vector2i(1000, 400));

    sf::RectangleShape MurDessin(sf::Vector2f(Size, Size));
    sf::Color mur(40, 40, 40);
    MurDessin.setFillColor(mur);

    

    sf::RectangleShape SucreDessin(sf::Vector2f(Size, Size));
    sf::Color sucre(244, 223, 177);
    SucreDessin.setFillColor(sucre);

    vector<sf::RectangleShape> ColoniesColor;
    for (int i=0; i<colonies.size(); i++) {
        sf::RectangleShape ColonieDessin(sf::Vector2f(Size, Size));
        int *Color = colonies[i]->getColor();
        sf::Color colonie(Color[0], Color[1], Color[2]);
        ColonieDessin.setFillColor(colonie);
        ColoniesColor.push_back(ColonieDessin);
    }

    Cell *Current;
    sf::RectangleShape CurrentShape(sf::Vector2f(Size, Size));
    Fourmi *ant;

    auto start = chrono::high_resolution_clock::now();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::Black);
        
        for (int y=0; y<Height; y++) {
            for (int x=0; x<Width; x++) {
                Current = terrain->getCell(Coord{x, y});
                if (Current->containsAnt()) {
                    writeToDebugFile("Ant", INFO_DETAIL);
                    // Get first ant that was on the cell
                    ant = Current->getToAnt()[0];
                    Colonie *c = ant->getColonie();
                    int Idx = c->getIdx();
                    CurrentShape = ColoniesColor[Idx];
                } else if (Current->containsNest()) {
                    writeToDebugFile("Nest", INFO_DETAIL);
                    int Idx = Current->getNestAbove()->getIdx();
                    CurrentShape = ColoniesColor[Idx];
                } else if (Current->containsSugar()) {
                    writeToDebugFile("Sugar", INFO_DETAIL);
                    CurrentShape = SucreDessin;
                } else if (Current->containsWall()) {
                    writeToDebugFile("Wall", INFO_DETAIL);
                    CurrentShape = MurDessin;
                } else if (Current->containsPheromone()) {
                    writeToDebugFile("Pheromone", ERROR);
                    map<int, int> Phero = Current->getPheromonesSucre();
                    int *color = new int[3]{0, 0, 0};
                    for (auto const& [key, val] : Phero) {
                        for (int i=0; i<3; i++) {
                            if (key==i) {
                                color[i] = val;
                            }
                        }
                    }
                    sf::Color pheromone(color[0], color[1], color[2]);
                    CurrentShape.setFillColor(pheromone);
                } else {
                    writeToDebugFile("Empty", INFO_DETAIL);
                    CurrentShape.setFillColor(sf::Color::Black);
                }
                CurrentShape.setPosition(x*Size, y*Size);
                window.draw(CurrentShape);
            }
        }

        window.display();

        nextTurn();

        while (chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() < 1000./(*VARIABLES["FPS"])) {
            // Wait
            usleep(1000);
        }
        start = chrono::high_resolution_clock::now();
    }
    writeToDebugFile("BoucleJeu fin", ERROR);
}


// Methode print screen

int main() {
    SetupVar();
    flushDebug();
    writeToDebugFile("main", INFO_DETAIL);
    srand(time(NULL));
    Jeu j{*VARIABLES["TAILLEGRILLE"], *VARIABLES["TAILLEGRILLE"], 3};
    j.BoucleJeuScreen();
    writeToDebugFile("main fin", ERROR);
    return 0;
}