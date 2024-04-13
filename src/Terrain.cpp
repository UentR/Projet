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
    for (Cell c : t.Cells) {
        int *C = t.toCoord(I);
        out << "(" << C[0] << ", " << C[1] << ")" << '\t' << c << endl;
        I++;
    }
    return out;
}

// Constructeur 

Cell::Cell(int Sugar, int State) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromones{} {}

#define DB_PERLIN_IMPL
#include "db_perlin.hpp"

Terrain::Terrain(int w, int h) : width{w}, height{h} {
    srand(time(0));
    Cells = {};
    int state;
    int PerlinNoise;
    int Sugar;
    int *C;
    for (int i=0; i<w*h; i++) {
        C = toCoord(i);
        Sugar = 0;
        
        PerlinNoise = rand()%100;

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

        

        Cell c{Sugar, state};
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

// Methode state cell

bool Cell::containsSugar() const {
    return sugarAmount>0;
}

bool Cell::containsNest() const {
    return (state%4) == 0;
}

bool Cell::containsNest(int Idx) const {
    return (state>>2) == Idx;
}

bool Cell::containsPheromone() const {
    for (auto const& [key, val] : pheromones) {
        return true;
    }
    return false;
}

bool Cell::containsPheromone(int a) const {
    return pheromones.count(a) != 0;
}

bool Cell::containsAnt() const {
    return toAnt.size() != 0;
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
    Cell Current{0, 0};
    for (int y=0; y<height; y++) {
        cout << endl << Vertical;
        for (int x=0; x<width; x++) {
            Current = Cells[Idx];
            if (Current.containsAnt()) {
                cout << " F ";
            } else if (Current.containsNest()) {
                cout << " N ";
            } else if (Current.containsSugar()) {
                cout << " S ";
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


int main() {
    Cell c{0, 0};
    cout << c << endl;
    Terrain t{44, 25};
    // cout << t << endl;

    t.toText();
    return 0;
}