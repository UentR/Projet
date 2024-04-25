#include "Terrain.hpp"

// cout

ostream &operator<<(ostream &out, Cell c) {
    out << "Sugar amount : " << c.sugarAmount << ", state : " << c.state << ", Ant above : ";
    for (Fourmi *f : c.toAnt) {
        out << f << " - ";
    }
    out << "Pheromones : ";
    for (auto const& [key, val] : c.pheromonesSucre) {
        out << " " << key << " - " << val << " | ";
    }
    return out;
}

ostream &operator<<(ostream &out, Terrain t) {
    int w = t.getWidth();
    out << "Width: " << w << ", Height: " << t.getHeight() << endl;
    int I = 0;
    for (Cell *c : t.Cells) {
        int *C = t.toCoord(I);
        out << "(" << C[0] << ", " << C[1] << ")" << '\t' << c << endl;
        I++;
    }
    return out;
}


// Constructeur 
Cell::Cell(int Sugar, int State, int Height, Coord Coordo) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromonesSucre{}, height{Height}, nestAbove{nullptr}, coord{Coordo} {}
Cell::Cell(int Sugar, int State, int Height) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromonesSucre{}, height{Height}, nestAbove{nullptr} {}

Cell::Cell(int Sugar, int State, int Height, Coord Coordo, Colonie *ptrCol) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromonesSucre{}, height{Height}, nestAbove{ptrCol}, coord{Coordo} {}

Cell::Cell() : sugarAmount{0}, state{2}, toAnt{}, pheromonesSucre{}, height{0} {}


Terrain::Terrain(int w, int h) : width{w}, height{h} {
    srand(time(0));
    Cells = {};
    int state;
    int PerlinNoise;
    int Sugar;
    int H = 0;
    int *C;
    Cell *c;
    for (int i=0; i<w*h; i++) {
        C = toCoord(i);
        Sugar = rand()%500;
        Sugar = max(0, Sugar-490);
        
        if (Sugar>0) {
            c = new Cell{Sugar, 1, H};  // 1 = SUGAR
        } else {
            if (rand()%70 == 0) {
                c = new Cell{0, 3, H};  // 3 = WALL
            } else {
                c = new Cell{0, 2, H};  // 2 = EMPTY
            }
        }

        // cout << &c << endl;
        Cells.push_back(c);
    }
}


// Methode Terrain

int Terrain::getWidth() const {
    return width;
}

int Terrain::getHeight() const {
    return height;
}

int *Terrain::toCoord(int Idx) const {
    int *Coord = new int[2];
    Coord[0] = Idx%getWidth();
    Coord[1] = Idx/getWidth();
    return Coord;
}

int Terrain::toIdx(int x, int y) const {
    return y*width + x;
}

Cell *Terrain::getCell(Coord c) const {
    return Cells[toIdx(c.getColonne(), c.getLigne())];
}

void Terrain::updateCell() {
    for (Cell *c : Cells) {
        c->update();
    }
}

// Methode Cell

Colonie *Cell::getNest() const {
    return nestAbove;
}

void Cell::update() {
    for (auto const& [key, val] : pheromonesSucre) {
        int newValue = val - REMOVEPHEROMONES;
        if (newValue <= 0) {
            pheromonesSucre.erase(key);
        } else {
            pheromonesSucre[key] = newValue;
        }
    }
}

int Cell::removeSugar(int Amount) {
    int newValue = sugarAmount - Amount;
    if (newValue <= 0) {
        state = 2;
        return sugarAmount;
    } else {
        sugarAmount = newValue;
        return Amount;
    }
}

unsigned short int Cell::getState() const {
    return state;
}


// Methode state cell

bool Cell::containsSugar() const {
    return sugarAmount>0;
}

bool Cell::containsNest() const {
    return nestAbove != nullptr;
}

bool Cell::containsNest(int Idx) const {
    return containsNest() && nestAbove->getIdx() == Idx;
}

bool Cell::isEmpty() const {
    return state==2;
}

bool Cell::containsNest(Colonie *c) const {
    return containsNest() && nestAbove == c;
}

bool Cell::containsPheromone() const {
    for (auto const& [key, val] : pheromonesSucre) {
        return true;
    }
    return false;
}

bool Cell::containsPheromone(unsigned int a) const {
    return pheromonesSucre.count(a) != 0;
}

bool Cell::containsPheromone(int a) const {
    for (auto const& [key, val] : pheromonesSucre) {
        if (key != a) {
            return true;
        }
    }
    return false;
}

bool Cell::containsAnt() const {
    return false;
    return toAnt.size() != 0;
}

bool Cell::containsWall() const {
    return state == 3;
}

bool Cell::setNest(Colonie *c) {
    if (containsNest()) {
        return false;
    }
    nestAbove = c;
    return true;
}


// int main() {
//     Cell c{0, 0, 0, Coord{0, 0}};
//     cout << c << endl;
//     Terrain t{44, 25};
//     // Terrain t{10, 10};
//     // cout << t << endl;

//     return 0;
// }