#include "Fourmi.hpp"
#include <iostream>
#include <string>

using namespace std;

// cout

ostream &operator<<(ostream &out, Fourmi *f) {
    writeToDebugFile("Entrée operator<< Fourmi", INFO_DETAIL);
    out << "Fourmi type : " << f->type << ", vie : " << f->vie << ", porte : " << f->quantiteSucre;
    writeToDebugFile("Sortie operator<< Fourmi", INFO_DETAIL);
    return out;
}

ostream &operator<<(ostream &out, Colonie c) { // ", position : " << c.getPosition() <<
    writeToDebugFile("Entrée operator<< Colonie", INFO_DETAIL);
    Coord coordo = c.getPosition();
    out << "Colonie : " << c.getIdx() << ", nbFourmis : " << c.getNbFourmis()
        << ", position : " << c.getPosition() << ", quantiteSucre : "
        << c.quantiteSucre << endl;
    for (Fourmi *fourmi : c.fourmis) {
        out << '\t' << fourmi << endl;
    }
    writeToDebugFile("Sortie operator<< Colonie", INFO_DETAIL);
    return out;
}


// Constructeurs

Fourmi::Fourmi() : colonieIdx{-1}, position{0, 0}, vision{1} {
    writeToDebugFile("Constructeur Fourmi", INFO_DETAIL);
    quantiteSucre = 0;
    isAttacked = 0;
    colonie = nullptr;
    writeToDebugFile("Constructeur Fourmi Fin", INFO_DETAIL);
}

Fourmi::Fourmi(Coord c, int Idx, Colonie *col, Terrain *t) : terrain{t}, colonieIdx{Idx}, position{c}, colonie{col}, vision{1} {
    writeToDebugFile("Constructeur Fourmi params", INFO_DETAIL);
    writeToDebugFile("Coord construct : " + to_string(c.getColonne()) + " " + to_string(c.getLigne()), INFO_DETAIL);
    quantiteSucre = 0;
    isAttacked = 0;
    writeToDebugFile("Constructeur Fourmi Fin", INFO_DETAIL);
}

Ouvriere::Ouvriere(Coord c, int Idx, Colonie *col, Terrain *t) : Fourmi(c, Idx, col, t) {
    writeToDebugFile("Constructeur Ouvriere", INFO_DETAIL);
    vie = VieO;
    type = 0;
    capaciteSucre = CapaciteSO;
    forceAttaque = AttaqueO;
    writeToDebugFile("Constructeur Ouvriere Fin", INFO_DETAIL);
}

Guerriere::Guerriere(Coord c, int Idx, Colonie *col, Terrain *t) : Fourmi(c, Idx, col, t) {
    writeToDebugFile("Constructeur Guerriere", INFO_DETAIL);
    vie = VieG;
    type = 1;
    capaciteSucre = CapaciteSG;
    forceAttaque = AttaqueG;
    writeToDebugFile("Constructeur Guerriere Fin", INFO_DETAIL);
}

Reproductrice::Reproductrice(Coord c, int Idx, Colonie *col, Terrain *t) : Ouvriere(c, Idx, col, t) {
    writeToDebugFile("Constructeur Reproductrice", INFO_DETAIL);
    vie = VieR;
    type = 2;
    capaciteSucre = CapaciteSR;
    forceAttaque = AttaqueR;
    writeToDebugFile("Constructeur Reproductrice Fin", INFO_DETAIL);
}

Colonie::Colonie(Terrain *t, Coord c, int i) : terrain{t}, nbFourmis{0}, typeFourmis{{0, 0, 0}}, quantiteSucre{0}, reproductriceEnAttente{0}, position{c} {
    writeToDebugFile("Constructeur Colonie", INFO_DETAIL);
    Idx = i;
    color = new int[3];
    writeToDebugFile("Couleur :", ALL_LOG);
    for (int i = 0; i < 3; i++) {
        color[i] = 255 * (i==Idx);
        writeToDebugFile(to_string(color[i]), ALL_LOG);
    }
    
    writeToDebugFile("Position colonie : " + to_string(c.getColonne()) + " " + to_string(c.getLigne()), ALL_LOG);
    vector<Cell *> voisins = terrain->voisin(c, SPAWNRADIUS);
    int Size = voisins.size();
    writeToDebugFile("Size : " + to_string(Size), ALL_LOG);

    Fourmi *f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NbF[i]; j++) {
            int rd = rand()%Size;
            writeToDebugFile("Cell idx: " + to_string(rd), ALL_LOG);
            Cell *cell = voisins[rd];
            Coord coord = cell->coord; 
            writeToDebugFile("Coord col constr : " + to_string(coord.getColonne()) + " " + to_string(coord.getLigne()), INFO_DETAIL);
            
            if (i == 0) {
                Ouvriere *o = new Ouvriere{coord, Idx, this, t};
                f = o;
            } else if (i == 1) {
                Guerriere *g = new Guerriere{coord, Idx, this, t};
                f = g;
            } else {
                Reproductrice *r = new Reproductrice{coord, Idx, this, t};
                f = r;
            }
            writeToDebugFile("Ajout Fourmi", ALL_LOG);
            typeFourmis[i]++;
            writeToDebugFile("Ajout Fourmi type : "+to_string(i), ALL_LOG);
            fourmis.push_back(f);
            writeToDebugFile("Ajout Fourmi nb: "+to_string(nbFourmis), ALL_LOG);
            cell->addAnt(f);
            writeToDebugFile("Add ant to cell", ALL_LOG);
            nbFourmis++;
            writeToDebugFile("Ajout fourmi fin", ALL_LOG);
        }
    }
    writeToDebugFile("Constructeur Colonie Fin", INFO_DETAIL);
}


// Méthodes Colonie

void Colonie::nextTurn() {
    writeToDebugFile("Next Turn", INFO_DETAIL);
    for (Fourmi *f : fourmis) {
        f->choixAction();
    }
    if (CheckReproduction()) {
        writeToDebugFile("Check Reproduction", ALL_LOG);
        produireFourmis();
    }
    writeToDebugFile("Next Turn Fin", INFO_DETAIL);
}


int Colonie::produireFourmis() {
    writeToDebugFile("Produire Fourmis", INFO_DETAIL);
    int comp = 0;
    for (int nb: typeFourmis){
        if (PourcentF[comp] > nb/nbFourmis) {
            writeToDebugFile("Produire Fourmis type : "+to_string(comp), INFO_DETAIL);
            if (comp == 0) {
                Ouvriere o{position, Idx, this, terrain};
                fourmis.push_back(&o);
            } else if (comp == 1) {
                Guerriere g{position, Idx, this, terrain};
                fourmis.push_back(&g);
            } else {
                Reproductrice r{position, Idx, this, terrain};
                fourmis.push_back(&r);
            }
            reproductriceEnAttente = {};
            typeFourmis[comp] += 1;
            nbFourmis += 1;
            return comp;
        }
        comp++;
    }
    writeToDebugFile("Produire Fourmis Fin", INFO_DETAIL);
    return -1;
}

int Colonie::getNbFourmis() const {
    writeToDebugFile("getNbFourmis", ALL_LOG);
    return nbFourmis;
}

int Colonie::getIdx() {
    writeToDebugFile("getIdx", ALL_LOG);
    return Idx;
}

Terrain *Colonie::getTerrain() {
    writeToDebugFile("getTerrain", ALL_LOG);
    return terrain;
}

Coord Colonie::getPosition() const {
    writeToDebugFile("getPosition", ALL_LOG);
    return position;
}

void Colonie::deleteFourmi(Fourmi *f) {
    writeToDebugFile("deleteFourmi", INFO_DETAIL);
    int i = 0;
    for (Fourmi *fourmi : fourmis) {
        if (fourmi == f) {
            fourmis.erase(fourmis.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi->type]--;
            delete f;
            writeToDebugFile("deleteFourmi Fin deleted : " + to_string(i), INFO_DETAIL);
            return;
        }
        i++;
    }
    writeToDebugFile("deleteFourmi Fin ant not found", INFO_DETAIL);
}

void Colonie::pretReproduction(Fourmi *f) {
    writeToDebugFile("pretReproduction", INFO_DETAIL);
    reproductriceEnAttente.insert(f);
    writeToDebugFile("pretReproduction Fin", INFO_DETAIL);
}

bool Colonie::CheckReproduction() {
    writeToDebugFile("CheckReproduction", INFO_DETAIL);
    if (reproductriceEnAttente.size() >= 2) {
        writeToDebugFile("CheckReproduction Fin True", INFO_DETAIL);
        return true;
    }
    writeToDebugFile("CheckReproduction Fin False", INFO_DETAIL);
    return false;
}

int *Colonie::getColor() {
    writeToDebugFile("getColor", ALL_LOG);
    writeToDebugFile("Color : "+to_string(color[0])+" "+to_string(color[1])+" "+to_string(color[2]), ALL_LOG);
    return color;
}

vector<int> Colonie::distanceNid(vector<Cell *> voisins) const {
    writeToDebugFile("distanceNid", INFO_DETAIL);
    vector<int> Score;
    for (Cell *c : voisins) {
        Score.push_back(c->coord.distance(position));
    }
    writeToDebugFile("distanceNid Fin", INFO_DETAIL);
    return Score;
}


// Methodes fourmis

void Fourmi::mettrePheromone() const {
    writeToDebugFile("mettrePheromone", INFO_DETAIL);
    terrain->getCell(position)->pheromonesSucre[colonie->getIdx()] = 255;
    writeToDebugFile("mettrePheromone Fin", INFO_DETAIL);
}

float Fourmi::attaquer(Fourmi *f) {
    writeToDebugFile("attaquer", INFO_DETAIL);
    f->isAttacked = NbTourAttaque;
    vie -= f->forceAttaque;
    f->vie -= forceAttaque;
    if (f->vie <= 0) {
        colonie->deleteFourmi(f);
    }
    if (vie <= 0) {
        colonie->deleteFourmi(this);
    }
    writeToDebugFile("attaquer Fin", INFO_DETAIL);
    return f->vie;
}

int Fourmi::ramasserSucre(Cell *cell) {
    writeToDebugFile("ramasserSucre", INFO_DETAIL);
    if (cell->containsSugar()) {
        int sugar = cell->removeSugar(capaciteSucre);
        quantiteSucre += sugar;
        writeToDebugFile("ramasserSucre Fin Sugar amount :" + to_string(sugar), INFO_DETAIL);
        return sugar;
    }
    writeToDebugFile("ramasserSucre Fin No Sugar", INFO_DETAIL);
    return 0;
}

bool Fourmi::deposerSucre(Coord c) {
    writeToDebugFile("deposerSucre", INFO_DETAIL);
    Cell *cell = terrain->getCell(c);
    if (cell->containsNest(colonieIdx)) {
        cell->sugarAmount += quantiteSucre;
        quantiteSucre = 0;
        writeToDebugFile("deposerSucre Fin add sugar", INFO_DETAIL);
        return true;
    }
    writeToDebugFile("deposerSucre Fin No Nest", INFO_DETAIL);
    return false;
}

bool Fourmi::deplacer(Cell *cell) {
    writeToDebugFile("deplacer", INFO_DETAIL);
    if (cell->isEmpty()) {
        terrain->getCell(position)->removeAnt(this);
        position = cell->coord;

        cell->addAnt(this);
        writeToDebugFile("deplacer Fin moved", INFO_DETAIL);
        return true;
    }
    writeToDebugFile("deplacer Fin not moved", INFO_DETAIL);
    return false;
}

vector<Fourmi *> Fourmi::fourmiProche() const {
    writeToDebugFile("fourmiProche", INFO_DETAIL);
    vector<Fourmi *> fourmisProches;
    vector<Cell *> voisinsCell = terrain->voisin(position, vision);
    for (Cell *Current : voisinsCell) {
        for (Fourmi *f : Current->toAnt) {
            if (f->colonie != colonie) {
                fourmisProches.push_back(f);
            }
        }
    }
    writeToDebugFile("fourmiProche Fin", INFO_DETAIL);
    return fourmisProches;
}

bool Fourmi::auNid() const {
    writeToDebugFile("auNid", INFO_DETAIL);
    vector<Coord> voisins = position.voisin(vision);
    vector<Cell *> voisinsCell = terrain->voisinState(position, vision, 0);
    for (Cell *c : voisinsCell) {
        if (c->nestAbove == colonie) {
            writeToDebugFile("auNid Fin True", INFO_DETAIL);
            return true;
        }
    }
    writeToDebugFile("auNid Fin False", INFO_DETAIL);
    return false;
}

Cell *Fourmi::procheState(int State) const {
    writeToDebugFile("procheState", INFO_DETAIL);
    writeToDebugFile("State : " + to_string(State), INFO_DETAIL);
    writeToDebugFile("Vision : " + to_string(vision), INFO_DETAIL);
    writeToDebugFile("Position : " + to_string(position.getColonne()) + " " + to_string(position.getLigne()), INFO_DETAIL);
    vector<Cell *> voisins = terrain->voisin(position, vision);
    Cell *Our = terrain->getCell(position);
    for (Cell *Current : voisins) {
        writeToDebugFile("State : " + to_string(Current->getState()), INFO_DETAIL);
        if (Current->getState() == State && Our!=Current) {
            writeToDebugFile("procheState Fin True", INFO_DETAIL);
            return Current;
        }
    }
    writeToDebugFile("procheState Fin False", INFO_DETAIL);
    return nullptr;
}

vector<Cell *> Fourmi::pheromonesProches(int Idx) const {
    writeToDebugFile("pheromonesProches", INFO_DETAIL);
    vector<Cell *> pheromonesProches;
    vector<Cell *> voisinsCell = terrain->voisin(position, vision);
    for (Cell *Current : voisinsCell) {
        if (Current->containsPheromone(Idx)) {
            pheromonesProches.push_back(Current);
        }
    }
    writeToDebugFile("pheromonesProches Fin", INFO_DETAIL);
    return pheromonesProches;
}

void Fourmi::choixAction() {
    writeToDebugFile("choixAction Fourmi", ALL_LOG);
    if (quantiteSucre>MinSucre) {
        if (auNid()) { // Done
            // Déposer sucre
            deposerSucre(colonie->getPosition());
        }
        else {
            
            // se déplacer vers le nid
            writeToDebugFile("choixAction Fourmi Fin NO ACTION sugar amount: " + to_string(quantiteSucre), ALL_LOG);
            vector<Cell *> voisins = terrain->voisin(position, vision);
            vector<int> Score = colonie->distanceNid(voisins);
            // get the index of the minimum element
            int MinIndex = min_element(Score.begin(), Score.end()) - Score.begin();
            Cell *voisin = voisins[MinIndex];
            Coord c = voisin->coord;
            writeToDebugFile("Mouv vers nid : " + to_string(c.getColonne()) + " " + to_string(c.getLigne()), ALL_LOG);
            deplacer(voisin);
            mettrePheromone();

        }
    } else {
        vector<Cell *> Sucres = terrain->voisinState(position, vision, 1);
        int Size = Sucres.size();
        if (Size != 0) { // Done
            // Ramasser sucre
            Cell *Sucre = Sucres[rand()%Size];
            ramasserSucre(Sucre);
            mettrePheromone();
        } else {
            vector<Cell *> pheromones = pheromonesProches(colonieIdx);
            if (pheromones.size() > 0) {
                // Suivre pheromone le plus loin du nid
                vector<int> Score = colonie->distanceNid(pheromones);
                int MaxIndex = max_element(Score.begin(), Score.end()) - Score.begin();
                Cell *voisin = pheromones[MaxIndex];
                deplacer(voisin);
            } else {  // FULL DONE
                // Se déplacer aléatoirement
                vector<Cell *> voisins = terrain->voisinState(position, vision, 2);
                int Size = voisins.size();
                if (Size != 0) {
                    Cell *voisin = voisins[rand()%Size];
                    Coord c = voisin->coord;
                    writeToDebugFile("Mouv aléatoire vers:" + to_string(c.getColonne()) + " " + to_string(c.getLigne()), ALL_LOG);
                    deplacer(voisin);
                } else {
                    writeToDebugFile("choixAction Fourmi Fin NO ACTION", WARNING);
                }
            }
        }
    }
    writeToDebugFile("choixAction Fourmi Fin", ALL_LOG);
}


void Guerriere::choixAction() {
    writeToDebugFile("choixAction Guerriere", ALL_LOG);
    vector<Fourmi *> fourmisProches = fourmiProche();
    if (fourmisProches.size() > 0) {
        // attaquer
        attaquer(fourmisProches[0]);
    } else {
        vector<Cell *> pheromones = pheromonesProches(-colonieIdx);
        if (pheromones.size() > 0) {
            // Suivre autre colonie
            deplacer(pheromones[0]);
        } else {
            // Comportement normal
            Fourmi::choixAction();
        }
    }
    writeToDebugFile("choixAction Guerriere Fin", ALL_LOG);
}

void Ouvriere::fuir() {
    writeToDebugFile("fuir", INFO_DETAIL);
    vector<Coord> voisins = position.voisin(vision);
    vector<Cell *> voisinsCell = terrain->voisinState(position, vision, 2);
    for (Cell *Current : voisinsCell) {
        deplacer(Current);
        writeToDebugFile("fuir Fin moved", INFO_DETAIL);
        return;
    }
    writeToDebugFile("fuir Fin ant blocked", INFO_DETAIL);
}


void Ouvriere::choixAction() {
    writeToDebugFile("choixAction Ouvriere", ALL_LOG);
    if (isAttacked > 0) {
        isAttacked--;
        // Fuir
        fuir();
    } else {
        // Comportement normal
        Fourmi::choixAction();
    }
    writeToDebugFile("choixAction Ouvriere Fin", ALL_LOG);
}

bool Reproductrice::checkReproduction() {
    writeToDebugFile("checkReproduction", INFO_DETAIL);
    if (colonie->quantiteSucre > MinSucre) {
        writeToDebugFile("checkReproduction Fin True", INFO_DETAIL);
        return true;
    }
    writeToDebugFile("checkReproduction Fin False", INFO_DETAIL);
    return false;
}


void Reproductrice::choixAction() {
    writeToDebugFile("choixAction Reproductrice", ALL_LOG);
    if (checkReproduction()) {
        // Reproduire
        if (auNid()) {
            colonie->pretReproduction(this);
        } else {
            // Retourner au nid
        }
    } else {
        // Comportement Ouvriere
        Ouvriere::choixAction();
    }
    writeToDebugFile("choixAction Reproductrice Fin", ALL_LOG);
}



// int main() {
//     Terrain t{10, 10};
//     Coord c{0, 1};
//     Colonie Col{&t, c};
//     cout << Col << endl;
//     return 0;
// }