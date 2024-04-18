#include "Terrain.hpp"

// cout

ostream &operator<<(ostream &out, Cell c) {
    out << "Sugar amount : " << c.sugarAmount << ", state : " << c.state << ", Ant above : ";
    for (Fourmi *f : c.toAnt) {
        out << f << " - ";
    }
    out << "Pheromones : ";
    for (auto const& [key, val] : c.pheromones) {
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
Cell::Cell(int Sugar, int State, int Height) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromones{}, height{Height}, nestAbove{nullptr} {}

Cell::Cell(int Sugar, int State, Colonie *ptrCol) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromones{}, height{0}, nestAbove{ptrCol} {}

Cell::Cell() : sugarAmount{0}, state{2}, toAnt{}, pheromones{}, height{0} {}


Terrain::Terrain(int w, int h) : width{w}, height{h} {
    srand(time(0));
    Cells = {};
    int state;
    int PerlinNoise;
    int Sugar;
    int H;
    int *C;
    for (int i=0; i<w*h; i++) {
        C = toCoord(i);
        Sugar = 0;
        
        PerlinNoise = rand()%100;
        H = rand()%20;

        // float PerlinNoise = (db::perlin(float(C[0])/w, float(C[1])/h) + 1)/2;
        if (PerlinNoise < 5) {
            state = 3;
        } else if (PerlinNoise < 93) {
            state = 2 + ((int)(PerlinNoise*64) << 2);
            state = 2;
        } else if (PerlinNoise < 99) {
            state = 1;
            Sugar = 100;
        } else {
            state = 0;
        }
        Cell c{Sugar, state, H};
        Cells.push_back(&c);
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


// Methode Cell

void Cell::update() {
    for (auto const& [key, val] : pheromones) {
        int newValue = val - REMOVEPHEROMONES;
        if (newValue <= 0) {
            pheromones.erase(key);
        } else {
            pheromones[key] = newValue;
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
    return nestAbove!=nullptr;
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
    for (auto const& [key, val] : pheromones) {
        return true;
    }
    return false;
}

bool Cell::containsPheromone(unsigned int a) const {
    return pheromones.count(a) != 0;
}

bool Cell::containsPheromone(int a) const {
    for (auto const& [key, val] : pheromones) {
        if (key != a) {
            return true;
        }
    }
    return false;
}

bool Cell::containsAnt() const {
    return toAnt.size() != 0;
}

bool Cell::containsWall() const {
    return state == 3;
}


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

void Terrain::toText() const {
    int LineSize = width;
    cout << UpRightC;
    repeatChar(LineSize-1, Horizontal+Horizontal+Horizontal+TUp);
    cout << Horizontal+Horizontal+Horizontal+UpLeftC;

    int Idx=0;
    Cell *Current;
    for (int y=0; y<height; y++) {
        cout << endl << Vertical;
        for (int x=0; x<width; x++) {
            Current = Cells[Idx];
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


// int main() {
//     Cell c{0, 0, 0};
//     cout << c << endl;
//     Terrain t{44, 25};
//     // cout << t << endl;

//     t.toText();
//     return 0;
// }