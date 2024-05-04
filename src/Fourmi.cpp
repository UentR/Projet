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
    Fourmi *fourmi;
    for (Ouvriere *f : c.ouvrieres) {
        fourmi = f;
        out << '\t' << fourmi << endl;
    }
    for (Guerriere *f : c.guerrieres) {
        fourmi = f;
        out << '\t' << fourmi << endl;
    }
    for (Reproductrice *f : c.reproductrices) {
        fourmi = f;
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
    vie = *VARIABLES["VieO"];
    type = 0;
    capaciteSucre = *VARIABLES["CapaciteSO"];
    forceAttaque = *VARIABLES["AttaqueO"];
    writeToDebugFile("Constructeur Ouvriere Fin", INFO_DETAIL);
}

Guerriere::Guerriere(Coord c, int Idx, Colonie *col, Terrain *t) : Fourmi(c, Idx, col, t) {
    writeToDebugFile("Constructeur Guerriere", INFO_DETAIL);
    vie = *VARIABLES["VieG"];
    type = 1;
    capaciteSucre = *VARIABLES["CapaciteSG"];
    forceAttaque = *VARIABLES["AttaqueG"];
    writeToDebugFile("Constructeur Guerriere Fin", INFO_DETAIL);
}

Reproductrice::Reproductrice(Coord c, int Idx, Colonie *col, Terrain *t) : Ouvriere(c, Idx, col, t) {
    writeToDebugFile("Constructeur Reproductrice", INFO_DETAIL);
    vie = *VARIABLES["VieR"];
    type = 2;
    capaciteSucre = *VARIABLES["CapaciteSR"];
    forceAttaque = *VARIABLES["AttaqueR"];
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
    vector<Cell *> voisins = terrain->voisin(c, *VARIABLES["SPAWNRADIUS"]);
    int Size = voisins.size();
    writeToDebugFile("Size : " + to_string(Size), ALL_LOG);

    Fourmi *f;
    int Count = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < VARIABLES["NbF"][i]; j++) {
            int rd = rand()%Size;
            writeToDebugFile("Cell idx: " + to_string(rd), ALL_LOG);
            Cell *cell = voisins[rd];
            Coord coord = cell->coord; 
            writeToDebugFile("Coord col constr : " + to_string(coord.getColonne()) + " " + to_string(coord.getLigne()), INFO_DETAIL);
            
            if (i == 0) {
                Ouvriere *o = new Ouvriere{coord, Idx, this, t};
                o->IDX = Count;
                ouvrieres.push_back(o);
                f = o;
                cell->addAnt(f);
            } else if (i == 1) {
                Guerriere *g = new Guerriere{coord, Idx, this, t};
                g->IDX = Count;
                guerrieres.push_back(g);
                f = g;
                cell->addAnt(f);
            } else {
                Reproductrice *r = new Reproductrice{coord, Idx, this, t};
                r->IDX = Count;
                reproductrices.push_back(r);
                f = r;
                cell->addAnt(f);
            }
            Count++;
            writeToDebugFile("Ajout Fourmi", ALL_LOG);
            typeFourmis[i]++;
            nbFourmis++;
            writeToDebugFile("Ajout fourmi fin", ALL_LOG);
        }
    }
    writeToDebugFile("Constructeur Colonie Fin", INFO_DETAIL);
}


// Méthodes Colonie

// Create a type that could be Ouvriere Guerriere ou Reproductrice



void Colonie::nextTurn() {
    writeToDebugFile("Next Turn", INFO_DETAIL);
    
    for (Ouvriere *o : ouvrieres) {
        writeToDebugFile("Ouvriere", ALL_LOG);
        writeToDebugFile("Sugar amount : " + to_string(o->quantiteSucre), ALL_LOG);
        writeToDebugFile("0 IDX : " + to_string(o->IDX), ALL_LOG);
        o->choixAction();
    }
    for (Guerriere *g : guerrieres) {
        writeToDebugFile("Guerriere", ALL_LOG);
        writeToDebugFile("Sugar amount : " + to_string(g->quantiteSucre), ALL_LOG);
        writeToDebugFile("0 IDX : " + to_string(g->IDX), ALL_LOG);
        g->choixAction();
    }
    for (Reproductrice *r : reproductrices) {
        writeToDebugFile("Reproductrice", ALL_LOG);
        writeToDebugFile("Sugar amount : " + to_string(r->quantiteSucre), ALL_LOG);
        writeToDebugFile("0 IDX : " + to_string(r->IDX), ALL_LOG);
        r->choixAction();
    }
    if (CheckReproduction()) {
        writeToDebugFile("Check Reproduction", INFO_DETAIL);
        produireFourmis();
    }
    writeToDebugFile("Next Turn Fin", INFO_DETAIL);
}


int Colonie::produireFourmis() {
    writeToDebugFile("Produire Fourmis", INFO_DETAIL);
    int comp = 0;
    vector<Cell *> voisins = terrain->voisin(position, *VARIABLES["SPAWNRADIUS"]);
    int Size = voisins.size();

    Fourmi *f;
    int Count = 20;
    for (int nb: typeFourmis){
        if (VARIABLES["PourcentF"][comp] > nb/nbFourmis) {
            int rd = rand()%Size;
            Cell *cell = voisins[rd];
            Coord coord = cell->coord; 
            if (comp == 0) {
                Ouvriere *o = new Ouvriere{coord, Idx, this, terrain};
                o->IDX = Count;
                ouvrieres.push_back(o);
                f = o;
                cell->addAnt(f);
            } else if (comp == 1) {
                Guerriere *g = new Guerriere{coord, Idx, this, terrain};
                g->IDX = Count;
                guerrieres.push_back(g);
                f = g;
                cell->addAnt(f);
            } else {
                Reproductrice *r = new Reproductrice{coord, Idx, this, terrain};
                r->IDX = Count;
                reproductrices.push_back(r);
                f = r;
                cell->addAnt(f);
            }
            reproductriceEnAttente.clear();
            writeToDebugFile("Ajout Fourmi", ALL_LOG);
            typeFourmis[comp]++;
            nbFourmis++;
            writeToDebugFile("Produire Fourmis type : "+to_string(comp), INFO_DETAIL);
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
    writeToDebugFile("deleteFourmi", ERROR);
    int i = 0;
    writeToDebugFile("Len ouvriere : " + to_string(ouvrieres.size()), ERROR);
    for (Ouvriere *fourmi : ouvrieres) {
        if (fourmi == f) {
            ouvrieres.erase(ouvrieres.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi->type]--;
            Cell *cell = terrain->getCell(fourmi->position);
            cell->removeAnt(fourmi);
            delete f;
            writeToDebugFile("deleteFourmi Fin deleted : " + to_string(i), ERROR);
            return;
        }
        i++;
    }
    i = 0;
    writeToDebugFile("Len guerriere : " + to_string(guerrieres.size()), ERROR);
    for (Guerriere *fourmi : guerrieres) {
        if (fourmi == f) {
            guerrieres.erase(guerrieres.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi->type]--;
            Cell *cell = terrain->getCell(fourmi->position);
            cell->removeAnt(fourmi);
            delete f;
            writeToDebugFile("deleteFourmi Fin deleted : " + to_string(i), ERROR);
            return;
        }
        i++;
    }
    i = 0;
    writeToDebugFile("Len reproductrice : " + to_string(reproductrices.size()), ERROR);
    for (Reproductrice *fourmi : reproductrices) {
        if (fourmi == f) {
            reproductrices.erase(reproductrices.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi->type]--;
            Cell *cell = terrain->getCell(fourmi->position);
            cell->removeAnt(fourmi);
            delete f;
            writeToDebugFile("deleteFourmi Fin deleted : " + to_string(i), ERROR);
            return;
        }
        i++;
    }
    writeToDebugFile("deleteFourmi Fin ant not found", ERROR);
}

void Colonie::pretReproduction(Reproductrice *f) {
    writeToDebugFile("pretReproduction", INFO_DETAIL);
    reproductriceEnAttente.insert(f);
    writeToDebugFile("pretReproduction Fin", INFO_DETAIL);
}

bool Colonie::CheckReproduction() {
    writeToDebugFile("CheckReproduction", INFO_DETAIL);
    return reproductriceEnAttente.size() >= 2;
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
    writeToDebugFile("attaquer", ERROR);
    f->isAttacked = *VARIABLES["NbTourAttaque"];
    vie -= f->forceAttaque;
    f->vie -= forceAttaque;
    if (f->vie <= 0) {
        writeToDebugFile("attaquer Fin DEAD them", ERROR);
        f->colonie->deleteFourmi(f);
    }
    if (vie <= 0) {
        writeToDebugFile("attaquer Fin DEAD us", ERROR);
        colonie->deleteFourmi(this);
    }
    writeToDebugFile("attaquer Fin", ERROR);
    return f->vie;
}

int Fourmi::ramasserSucre(Cell *cell) {
    writeToDebugFile("ramasserSucre", INFO_DETAIL);
    if (cell->containsSugar()) {
        int sugar = cell->removeSugar(capaciteSucre);
        quantiteSucre += sugar;
        writeToDebugFile("ramasserSucre Fin Sugar amount :" + to_string(sugar), INFO_DETAIL);
        writeToDebugFile("ramasserSucre Fin Sugar amount fourmi :" + to_string(quantiteSucre), INFO_DETAIL);
        writeToDebugFile("2 IDX : " + to_string(IDX), INFO_DETAIL);
        return sugar;
    }
    writeToDebugFile("ramasserSucre Fin No Sugar", INFO_DETAIL);
    return 0;
}

bool Fourmi::deposerSucre(Coord c) {
    writeToDebugFile("deposerSucre", INFO_DETAIL);
    Cell *cell = terrain->getCell(c);
    if (cell->containsNest(colonieIdx)) {
        colonie->quantiteSucre += quantiteSucre;
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
    int dist = position.distance(colonie->getPosition());
    return dist <= 1;
    // vector<Coord> voisins = position.voisin(vision);
    // vector<Cell *> voisinsCell = terrain->voisinState(position, vision, 0);
    // for (Cell *c : voisinsCell) {
    //     if (c->nestAbove == colonie) {
    //         writeToDebugFile("auNid Fin True", INFO_DETAIL);
    //         return true;
    //     }
    // }
    // writeToDebugFile("auNid Fin False", INFO_DETAIL);
    // return false;
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

void Fourmi::choixActionF() {
    writeToDebugFile("choixAction Fourmi", INFO_DETAIL);
    writeToDebugFile("Sugar amount : " + to_string(quantiteSucre), ALL_LOG);
    writeToDebugFile("3 IDX : " + to_string(IDX), ALL_LOG);
    if (quantiteSucre>*VARIABLES["MinSucre"]) {
        if (auNid()) { // Done
            // Déposer sucre
            writeToDebugFile("choixAction Fourmi Fin DEPOSE SUCRE sugar amount fourmi: " + to_string(quantiteSucre), ALL_LOG);
            deposerSucre(colonie->getPosition());
            writeToDebugFile("choixAction Fourmi Fin DEPOSE SUCRE sugar amount colonie: " + to_string(colonie->quantiteSucre), ALL_LOG);
            writeToDebugFile("choixAction Fourmi Fin DEPOSE SUCRE sugar amount fourmi: " + to_string(quantiteSucre), ALL_LOG);
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
            writeToDebugFile("Mouv vers nid fin", ALL_LOG);
            mettrePheromone();
            writeToDebugFile("Mettre pheromone fin", ALL_LOG);

        }
    } else {
        vector<Cell *> Sucres = terrain->voisinState(position, vision, 1);
        int Size = Sucres.size();
        if (Size != 0) { // Done
            writeToDebugFile("choixAction Fourmi RAMASSER SUCRE", INFO_DETAIL);
            // Ramasser sucre
            Cell *Sucre = Sucres[rand()%Size];
            ramasserSucre(Sucre);
            mettrePheromone();
        } else {
            vector<Cell *> pheromones = pheromonesProches(colonieIdx);
            if (pheromones.size() > 0) {
                writeToDebugFile("choixAction Fourmi SUIVRE PHEROMONE", INFO_DETAIL);
                // Suivre pheromone le plus loin du nid
                vector<int> Score = colonie->distanceNid(pheromones);
                int MaxIndex = max_element(Score.begin(), Score.end()) - Score.begin();
                Cell *voisin = pheromones[MaxIndex];
                deplacer(voisin);
            } else {  // FULL DONE
                writeToDebugFile("choixAction Fourmi MOUV ALEATOIRE", INFO_DETAIL);
                // Se déplacer aléatoirement
                vector<Cell *> voisins = terrain->voisinState(position, vision, 2);
                int Size = voisins.size();
                if (Size != 0) {
                    Cell *voisin = voisins[rand()%Size];
                    Coord c = voisin->coord;
                    writeToDebugFile("Mouv aléatoire vers:" + to_string(c.getColonne()) + " " + to_string(c.getLigne()), INFO_DETAIL);
                    deplacer(voisin);
                } else {
                    writeToDebugFile("choixAction Fourmi Fin NO ACTION", WARNING);
                }
            }
        }
    }
    writeToDebugFile("choixAction Fourmi Fin", INFO_DETAIL);
}

void Guerriere::choixAction() {
    writeToDebugFile("choixAction Guerriere", INFO);
    vector<Fourmi *> fourmisProches = fourmiProche();
    if (fourmisProches.size() > 0) {
        writeToDebugFile("choixAction Guerriere Attaque", INFO);
        // attaquer
        attaquer(fourmisProches[0]);
    } else {
        vector<Cell *> pheromones = pheromonesProches(-colonieIdx);
        if (pheromones.size() > 0) {
            writeToDebugFile("choixAction Guerriere Suivre Pheromone", INFO);
            // Suivre autre colonie
            deplacer(pheromones[0]);
        } else {
            // Comportement normal
            Fourmi::choixActionF();
        }
    }
    writeToDebugFile("choixAction Guerriere Fin", INFO_DETAIL);
}

void Ouvriere::fuir() {
    writeToDebugFile("fuir", INFO_DETAIL);
    vector<Cell *> voisins = terrain->voisinState(position, vision, 2);
    int Size = voisins.size();
    if (Size != 0) {
        Cell *voisin = voisins[rand()%Size];
        Coord c = voisin->coord;
        writeToDebugFile("Mouv aléatoire vers:" + to_string(c.getColonne()) + " " + to_string(c.getLigne()), INFO_DETAIL);
        deplacer(voisin);
    } else {
        writeToDebugFile("choixAction Fourmi NO ACTION", WARNING);
    }
    writeToDebugFile("fuir Fin ant moved: " + to_string(isAttacked), INFO_DETAIL);
}

void Ouvriere::choixAction() {
    writeToDebugFile("choixAction Ouvriere", INFO);
    if (isAttacked > 0) {
        writeToDebugFile("choixAction Ouvriere est Attaque", INFO_DETAIL);
        isAttacked--;
        // Fuir
        fuir();
    } else {
        // Comportement normal
        Fourmi::choixActionF();
    }
    writeToDebugFile("choixAction Ouvriere Fin", INFO_DETAIL);
}

bool Reproductrice::checkReproduction() {
    writeToDebugFile("checkReproduction", INFO_DETAIL);
    if (colonie->quantiteSucre > *VARIABLES["MinSucre"]) {
        writeToDebugFile("checkReproduction Fin True", INFO_DETAIL);
        return true;
    }
    writeToDebugFile("checkReproduction Fin False", INFO_DETAIL);
    return false;
}

void Reproductrice::choixAction() {
    writeToDebugFile("choixAction Reproductrice", INFO);
    if (checkReproduction()) {
        writeToDebugFile("Sufficent sugar", INFO_DETAIL);
        // Reproduire
        if (auNid()) {
            colonie->pretReproduction(this);
        } else {
            // Retourner au nid
            vector<Cell *> voisins = terrain->voisin(position, vision);
            vector<int> Score = colonie->distanceNid(voisins);
            // get the index of the minimum element
            int MinIndex = min_element(Score.begin(), Score.end()) - Score.begin();
            Cell *voisin = voisins[MinIndex];
            deplacer(voisin);
        }
    } else {
        // Comportement Ouvriere
        Ouvriere::choixAction();
    }
    writeToDebugFile("choixAction Reproductrice Fin", INFO_DETAIL);
}



// int main() {
//     Terrain t{10, 10};
//     Coord c{0, 1};
//     Colonie Col{&t, c};
//     cout << Col << endl;
//     return 0;
// }