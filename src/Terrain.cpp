#include "Terrain.hpp"
#include <algorithm>

#include <parallel/algorithm>
#include <execution>


// cout

ostream &operator<<(ostream &out, Cell c) {
    writeToDebugFile("Entrée operator<< Cell", INFO_DETAIL);
    out << "Sugar amount : " << c.getSugarAmount() << ", state : " << c.getState() << ", Ant above : ";
    for (Fourmi *f : c.getToAnt()) {
        out << f << " - ";
    }
    out << "Pheromones : ";
    for (auto const& [key, val] : c.getPheromonesSucre()) {
        out << " " << key << " - " << val << " | ";
    }
    out << "Nest above : " << c.getNestAbove();
    writeToDebugFile("Sortie operator<< Cell", INFO_DETAIL);
    return out;
}

ostream &operator<<(ostream &out, Terrain t) {
    writeToDebugFile("Entrée operator<< Terrain", INFO_DETAIL);
    int w = t.getWidth();
    out << "Width: " << w << ", Height: " << t.getHeight() << endl;
    int I = 0;
    for (Cell *c : t.getCells()) {
        int *C = t.toCoord(I);
        out << "(" << C[0] << ", " << C[1] << ")" << '\t' << *c << endl;
        I++;
    }
    writeToDebugFile("Sortie operator<< Terrain", INFO_DETAIL);
    return out;
}


// Constructeur 
Cell::Cell(int Sugar, int State, int Height, Coord Coordo) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromonesSucre{}, height{Height}, nestAbove{nullptr}, coord{Coordo} {}

Cell::Cell(int Sugar, int State, int Height, Coord Coordo, Colonie *ptrCol) : sugarAmount{Sugar}, state{State}, toAnt{}, pheromonesSucre{}, height{Height}, nestAbove{ptrCol}, coord{Coordo} {}

Cell::Cell() : sugarAmount{0}, state{2}, toAnt{}, pheromonesSucre{}, height{0} {}


Terrain::Terrain(int w, int h) : width{w}, height{h} {
    writeToDebugFile("Entrée Constructeur Terrain", INFO_DETAIL);
    srand(time(0));
    Cells = {};
    int Sugar;
    int Wall;
    int H = 0;
    Cell *c;
    int *Coordo = new int[2];
    Coord C;
    for (int i=0; i<w*h; i++) {
        // C = toCoord(i);
        Sugar = rand()%int(100*(*VARIABLES["MaxSucre"])/(*VARIABLES["ChanceSucre"]));
        Sugar = -min(Sugar-(*VARIABLES["MaxSucre"]), float(0));
        
        Coordo = toCoord(i);
        C = Coord{Coordo[0], Coordo[1]};

        if (Sugar>0) {
            c = new Cell{Sugar*10, 1, H, C};  // 1 = SUGAR
        } else {
            if (rand()%int(100/(*VARIABLES["ChanceMur"])) == 0) {
                c = new Cell{0, 3, H, C};  // 3 = WALL
            } else {
                c = new Cell{0, 2, H, C};  // 2 = EMPTY
            }
        }

        // cout << &c << endl;
        writeToDebugFile("Coord terrain : " + to_string(C.getColonne()) + " " + to_string(C.getLigne()), ALL_LOG);
        Cells.push_back(c);
    }
    writeToDebugFile("Sortie Constructeur Terrain", INFO_DETAIL);
}


// Methode Terrain

int Terrain::getWidth() const {
    writeToDebugFile("Entrée getWidth", ALL_LOG);
    return width;
}

int Terrain::getHeight() const {
    writeToDebugFile("Entrée getHeight", ALL_LOG);
    return height;
}

int *Terrain::toCoord(int Idx) const {
    writeToDebugFile("Entrée toCoord", ALL_LOG);
    int *Coord = new int[2];
    Coord[0] = Idx%width;
    Coord[1] = Idx/width;
    writeToDebugFile("Coord : " + to_string(Coord[0]) + " " + to_string(Coord[1]), ALL_LOG);
    writeToDebugFile("Sortie toCoord", ALL_LOG);
    return Coord;
}

int Terrain::toIdx(int x, int y) const {
    writeToDebugFile("Entrée toIdx", ALL_LOG);
    return y*width + x;
}

vector<Cell *> Terrain::getCells() const {
    writeToDebugFile("Entrée getCells", ALL_LOG);
    return Cells;
}

Cell *Terrain::getCell(Coord c) const {
    writeToDebugFile("Entrée getCell", ALL_LOG);
    return Cells[toIdx(c.getColonne(), c.getLigne())];
}

Cell *Terrain::getCell(int c) const {
    writeToDebugFile("Entrée toIdx", ALL_LOG);
    return Cells[c];
}

void Terrain::updateCell() {
    writeToDebugFile("Entrée updateCell", INFO_DETAIL);
    __gnu_parallel::for_each(Cells.begin(), Cells.end(), [](auto&& c) {
    // for (Cell *c : Cells) {
        c->update();
    });
    // }
    writeToDebugFile("Sortie updateCell", INFO_DETAIL);
}

vector<Cell *> Terrain::voisin(Coord c, int Rayon) const {
    writeToDebugFile("Entrée voisin", INFO_DETAIL);
    vector<Cell *> voisins;
    int ind_col = c.getColonne();
    int ind_lig = c.getLigne();
    int DebX = max(0, ind_col-Rayon);
    int MaxX = min(width-1, ind_col+Rayon);
    int DebY = max(0, ind_lig-Rayon);
    int MaxY = min(height-1, ind_lig+Rayon);
    Cell *cell;
    for (int x = DebX; x <= MaxX; x++) {
        for (int y = DebY; y <= MaxY; y++) {
            if (x!=ind_col or y!=ind_lig) {
                cell = getCell(Coord{x, y});
                if (cell->isEmpty()) {
                    writeToDebugFile("Coord terrain : " + to_string(x) + " " + to_string(y), ALL_LOG);
                    voisins.push_back(cell);
                }
            }
        }
    }
    writeToDebugFile("Sortie voisin", INFO_DETAIL);
    return voisins;
}

vector<Cell *> Terrain::voisinState(Coord c, int Rayon, int State) const {
    writeToDebugFile("Entrée voisin", INFO_DETAIL);
    vector<Cell *> voisins;
    int ind_col = c.getColonne();
    int ind_lig = c.getLigne();
    int DebX = max(0, ind_col-Rayon);
    int MaxX = min(width-1, ind_col+Rayon);
    int DebY = max(0, ind_lig-Rayon);
    int MaxY = min(height-1, ind_lig+Rayon);
    Cell *cell;
    for (int x = DebX; x <= MaxX; x++) {
        for (int y = DebY; y <= MaxY; y++) {
            if (x!=ind_col or y!=ind_lig) {
                cell = getCell(Coord{x, y});
                if (cell->getState() == State) {
                    writeToDebugFile("Coord terrain : " + to_string(x) + " " + to_string(y), ALL_LOG);
                    voisins.push_back(cell);
                }
            }
        }
    }
    writeToDebugFile("Sortie voisin", INFO_DETAIL);
    return voisins;
}

// Methode Cell

bool Cell::setPheromonesSucre(int Idx, int Value) {
    writeToDebugFile("Entrée setPheromonesSucre", INFO_DETAIL);
    pheromonesSucre[Idx] = Value;
    return true;
}

void Cell::removeAnt(Fourmi *f) {
    writeToDebugFile("Entrée removeAnt", INFO_DETAIL);
    // remove f from toAnt
    for (int i = 0; i < toAnt.size(); i++) {
        if (toAnt[i] == f) {
            toAnt.erase(toAnt.begin() + i);
            writeToDebugFile("Sortie removeAnt ant found", INFO_DETAIL);
            return;
        }
    }
    writeToDebugFile("Sortie removeAnt ant not found", INFO_DETAIL);
}

void Cell::addAnt(Fourmi *f) {
    writeToDebugFile("Entrée addAnt", INFO_DETAIL);
    writeToDebugFile("Position:" + to_string(coord.getColonne()) + " " + to_string(coord.getLigne()), ALL_LOG);
    writeToDebugFile("Fourmi ajoutée nb:" + to_string(toAnt.size()), ALL_LOG);
    toAnt.push_back(f);
    writeToDebugFile("Sortie addAnt", INFO_DETAIL);
}

void Cell::update() {
    writeToDebugFile("Entrée update", ALL_LOG);
    if (pheromonesSucre.size() == 0) {
        writeToDebugFile("Sortie update NO UPDATE", ALL_LOG);
        return;
    }
    vector<int> toDelete;
    for (auto const& [key, val] : pheromonesSucre) {
        int newValue = val - REMOVEPHEROMONES;
        if (newValue <= 0) {
            writeToDebugFile("Pheromone deleted", ALL_LOG);
            writeToDebugFile("Size pheromones: " + to_string(pheromonesSucre.size()), ALL_LOG);
            writeToDebugFile("Key: " + to_string(key), ALL_LOG);
            writeToDebugFile("Value: " + to_string(pheromonesSucre[key]), ALL_LOG);
            toDelete.push_back(key);
        } else {
            writeToDebugFile("Pheromone updated", ALL_LOG);
            pheromonesSucre[key] = newValue;
        }
    }
    for (int k : toDelete) {
        pheromonesSucre.erase(k);
    }
    writeToDebugFile("Sortie update", ALL_LOG);
}

int Cell::removeSugar(int Amount) {
    writeToDebugFile("Entrée removeSugar", INFO_DETAIL);
    int newValue = sugarAmount - Amount;
    if (newValue <= 0) {
        int Temp = sugarAmount;
        sugarAmount = 0;
        state = 2;
        writeToDebugFile("Sortie removeSugar convert Nest", INFO_DETAIL);
        writeToDebugFile("Sugar amount <0: " + to_string(sugarAmount), INFO_DETAIL);
        return Temp;
    } else {
        sugarAmount = newValue;
        writeToDebugFile("Sortie removeSugar: " + to_string(sugarAmount), INFO_DETAIL);
        writeToDebugFile("Sugar amount >0 : " + to_string(sugarAmount), INFO_DETAIL);
        return Amount;
    }
}


// Methode state cell

Coord Cell::getCoord() const {
    writeToDebugFile("Entrée getCoord", ALL_LOG);
    return coord;
}

map<int, int> Cell::getPheromonesSucre() const {
    writeToDebugFile("Entrée getPheromonesSucre", ALL_LOG);
    return pheromonesSucre;
}

int Cell::getSugarAmount() const {
    writeToDebugFile("Entrée getSugarAmount", ALL_LOG);
    return sugarAmount;
}

vector<Fourmi *> Cell::getToAnt() const {
    writeToDebugFile("Entrée getToAnt", ALL_LOG);
    return toAnt;
}


int Cell::getState() const {
    writeToDebugFile("Entrée getState", ALL_LOG);
    return state;
}

Colonie *Cell::getNestAbove() const {
    writeToDebugFile("Entrée containsSugar", ALL_LOG);
    return nestAbove;
}

bool Cell::containsSugar() const {
    writeToDebugFile("Entrée containsSugar", ALL_LOG);
    return sugarAmount>0;
}

bool Cell::containsNest() const {
    writeToDebugFile("Entrée containsNest 0", ALL_LOG);
    return state == 0;
}

bool Cell::containsNest(int Idx) const {
    writeToDebugFile("Entrée containsNest 1", ALL_LOG);
    return containsNest() && nestAbove->getIdx() == Idx;
}

bool Cell::isEmpty() const {
    writeToDebugFile("Entrée isEmpty", ALL_LOG);
    return state==2;
}

bool Cell::containsNest(Colonie *c) const {
    writeToDebugFile("Entrée containsNest 2", ALL_LOG);
    return nestAbove == c;
}

bool Cell::containsPheromone() const {
    writeToDebugFile("Entrée containsPheromone", ALL_LOG);
    for (auto const& [key, val] : pheromonesSucre) {
        return true;
    }
    return false;
}

bool Cell::containsPheromone(unsigned int a) const {
    writeToDebugFile("Entrée containsPheromone our", ALL_LOG);
    return pheromonesSucre.count(a) != 0;
}

bool Cell::containsPheromone(int a) const {
    writeToDebugFile("Entrée containsPheromone other", ALL_LOG);
    if (pheromonesSucre.size() == 0) {
        writeToDebugFile("Sortie containsPheromone False", ALL_LOG);
        return false;
    }
    for (auto const& [key, val] : pheromonesSucre) {
        if (key != a) {
            writeToDebugFile("Sortie containsPheromone True", ALL_LOG);
            return true;
        }
    }
    writeToDebugFile("Sortie containsPheromone False", ALL_LOG);
    return false;
}

bool Cell::containsAnt() const {
    writeToDebugFile("Entrée containsAnt", ALL_LOG);
    return toAnt.size() != 0;
}

bool Cell::containsWall() const {
    writeToDebugFile("Entrée containsWall", ALL_LOG);
    return state == 3;
}

bool Cell::setNest(Colonie *c) {
    writeToDebugFile("Entrée setNest", ALL_LOG);
    if (containsNest()) {
        writeToDebugFile("Sortie setNest False", ALL_LOG);
        return false;
    }
    state = 0;
    nestAbove = c;
    writeToDebugFile("Sortie setNest True", ALL_LOG);
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