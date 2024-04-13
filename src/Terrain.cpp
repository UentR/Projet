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

Terrain::Terrain(int w, int h) : width{w}, height{h} {
    Cells = {};
    for (int i=0; i<w*h; i++) {
        Cell c{0, 0};
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

void Terrain::toText() const {
    int LineSize = 2*width - 1;
    cout << " ";
    repeatChar(LineSize, '-');
    cout << endl;

    int Idx=0;
    Cell Current{0, 0};
    for (int y=0; y<height; y++) {
        cout << "|";
        for (int x=0; x<width; x++) {
            Current = Cells[Idx];
            if (Current.containsAnt()) {
                cout << "F";
            } else if (Current.containsNest()) {
                cout << "N";
            } else if (Current.containsSugar()) {
                cout << "S";
            } else {
                cout << " ";
            }
            cout << "|";
            Idx++;
        }
        cout << endl << " ";
        repeatChar(LineSize, '-');
        cout << endl;
    }
}





int main() {
    Cell c{0, 0};
    cout << c << endl;
    Terrain t{10, 10};
    cout << t << endl;

    t.toText();

    return 0;
}