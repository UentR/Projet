#include "Fourmi.hpp"
#include <iostream>
#include <string>

using namespace std;

// cout

ostream &operator<<(ostream &out, Fourmi *f) {
    out << "Fourmi type : " << f->type << ", vie : " << f->vie << ", porte : " << f->quantiteSucre;
    return out;
}

ostream &operator<<(ostream &out, Colonie c) { // ", position : " << c.getPosition() <<
    Coord coordo = c.getPosition();
    out << "Colonie : " << c.getIdx() << ", nbFourmis : " << c.getNbFourmis()
        << ", position : " << c.getPosition() << ", quantiteSucre : "
        << c.quantiteSucre << endl;
    for (Fourmi *fourmi : c.fourmis) {
        out << '\t' << fourmi << endl;
    }
    return out;
}


// Constructeurs

Fourmi::Fourmi() : colonieIdx{-1}, position{0, 0}, vision{1} {
    quantiteSucre = 0;
    isAttacked = 0;
    colonie = nullptr;
}

Fourmi::Fourmi(Coord c, int Idx, Colonie *col, Terrain *t) : terrain{t}, colonieIdx{Idx}, position{c}, colonie{col}, vision{1} {
    quantiteSucre = 0;
    isAttacked = 0;
}

Ouvriere::Ouvriere(Coord c, int Idx, Colonie *col, Terrain *t) : Fourmi(c, Idx, col, t) {
    vie = VieO;
    type = 0;
    capaciteSucre = CapaciteSO;
    forceAttaque = AttaqueO;
}

Guerriere::Guerriere(Coord c, int Idx, Colonie *col, Terrain *t) : Fourmi(c, Idx, col, t) {
    vie = VieG;
    type = 1;
    capaciteSucre = CapaciteSG;
    forceAttaque = AttaqueG;
}

Reproductrice::Reproductrice(Coord c, int Idx, Colonie *col, Terrain *t) : Ouvriere(c, Idx, col, t) {
    vie = VieR;
    type = 2;
    capaciteSucre = CapaciteSR;
    forceAttaque = AttaqueR;
}

Colonie::Colonie(Terrain *t, Coord c, int i) : terrain{t}, nbFourmis{0}, typeFourmis{{0, 0, 0}}, quantiteSucre{0}, reproductriceEnAttente{0}, position{c} {
    Idx = i;
    color = new int[3];
    for (int i = 0; i < 3; i++) {
        color[i] = 255 * (i==Idx);
    }
    
    Fourmi *f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NbF[i]; j++) {
            if (i == 0) {
                Ouvriere *o = new Ouvriere{c, Idx, this, t};
                f = o;
            } else if (i == 1) {
                Guerriere *g = new Guerriere{c, Idx, this, t};
                f = g;
            } else {
                Reproductrice *r = new Reproductrice{c, Idx, this, t};
                f = r;
            }
            typeFourmis[i]++;
            fourmis.push_back(f);
            nbFourmis++;
        }
    }
}


// Méthodes Colonie

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
    return nbFourmis;
}

int Colonie::getIdx() {
    return Idx;
}

Terrain *Colonie::getTerrain() {
    return terrain;
}

Coord Colonie::getPosition() const {
    return position;
}

void Colonie::deleteFourmi(Fourmi *f) {
    int i = 0;
    for (Fourmi *fourmi : fourmis) {
        if (fourmi == f) {
            fourmis.erase(fourmis.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi->type]--;
            return;
        }
        i++;
    }
}

void Colonie::pretReproduction(Fourmi *f) {
    reproductriceEnAttente.insert(f);
}

bool Colonie::CheckReproduction() {
    if (reproductriceEnAttente.size() >= 2) {
        return true;
    }
    return false;
}

int *Colonie::getColor() {
    return color;
}

// Methodes fourmis

void Fourmi::mettrePheromone() const {
    terrain->Cells[terrain->toIdx(position.getColonne(), position.getLigne())]->pheromonesSucre[colonie->getIdx()] = PosePheromones;
}

float Fourmi::attaquer(Fourmi *f) {
    f->isAttacked = NbTourAttaque;
    vie -= f->forceAttaque;
    f->vie -= forceAttaque;
    return f->vie;
}

int Fourmi::ramasserSucre(Coord c) {
    int Idx = terrain->toIdx(c.getColonne(), c.getLigne());
    if (terrain->Cells[Idx]->containsSugar()) {
        int sugar = terrain->Cells[Idx]->removeSugar(capaciteSucre);
        quantiteSucre += sugar;
        return sugar;
    }
    return 0;
}

bool Fourmi::deposerSucre(Coord c) {
    int Idx = terrain->toIdx(c.getColonne(), c.getLigne());
    if (terrain->Cells[Idx]->containsNest(colonieIdx)) {
        terrain->Cells[Idx]->sugarAmount += quantiteSucre;
        quantiteSucre = 0;
        return true;
    }
    return false;
}

bool Fourmi::deplacer(Coord c) {
    int Idx = terrain->toIdx(c.getColonne(), c.getLigne());
    if (terrain->Cells[Idx]->isEmpty()) {
        position = c;

        // ENLEVER FOURMI DE LA CELLULE PRECEDENTE
        // Ajouter fourmi à la nouvelle cellule


        return true;
    }
    return false;
}

vector<Fourmi *> Fourmi::fourmiProche() const {
    vector<Fourmi *> fourmisProches;
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        Cell *Current = terrain->getCell(c);
        for (Fourmi *f : Current->toAnt) {
            if (f->colonie != colonie) {
                fourmisProches.push_back(f);
            }
        }
    }
    return fourmisProches;
}

bool Fourmi::auNid() const {
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        if (c == colonie->getPosition()) {
            return true;
        }
    }
    return false;
}

Coord Fourmi::procheState(int State) const {
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        Cell *Current = terrain->getCell(c);
        if (Current->getState() == State) {
            return c;
        }
    }
    return Coord{-1, -1};
}

vector<Coord> Fourmi::pheromonesProches(int Idx) const {
    vector<Coord> pheromonesProches;
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        Cell *Current = terrain->getCell(c);
        if (Current->containsPheromone(Idx)) {
            pheromonesProches.push_back(c);
        }
    }
    return pheromonesProches;
}

void Fourmi::choixAction() {
    if (quantiteSucre>MinSucre) {
        if (auNid()) { // Done
            // Déposer sucre
            deposerSucre(colonie->getPosition());
        }
        else {
            
            // se déplacer vers le nid

        }
    } else {
        Coord Sucre = procheState(1);
        if (Sucre != (Coord){-1, -1}) { // Done
            // Ramasser sucre
            ramasserSucre(Sucre);
            mettrePheromone();
        } 
        else {
            vector<Coord> pheromones = pheromonesProches(colonieIdx);
            if (pheromones.size() > 0) {
                // Suivre pheromone
                deplacer(pheromones[0]);
            } else {  // Mid Done choix aléatoire
                // Se déplacer aléatoirement
                Coord voisin = procheState(2);
                deplacer(voisin);
            }
        }
    }

}


void Guerriere::actionGuerriere() {
    vector<Fourmi *> fourmisProches = fourmiProche();
    if (fourmisProches.size() > 0) {
        // attaquer
        attaquer(fourmisProches[0]);
    } else {
        vector<Coord> pheromones = pheromonesProches(-colonieIdx);
        if (pheromones.size() > 0) {
            // Suivre autre colonie
            deplacer(pheromones[0]);
        } else {
            // Comportement normal
            choixAction();
        }
    }
}

void Ouvriere::fuir() {
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        Cell *Current = terrain->getCell(c);
        if (Current->isEmpty()) {
            deplacer(c);
            return;
        }
    }
}


void Ouvriere::actionOuvriere() {
    if (isAttacked > 0) {
        isAttacked--;
        // Fuir
        fuir();
    } else {
        // Comportement normal
        choixAction();
    }
}

bool Reproductrice::checkReproduction() {
    if (colonie->quantiteSucre > MinSucre) {
        return true;
    }
    return false;
}


void Reproductrice::actionReproductrice() {
    if (checkReproduction()) {
        // Reproduire
        if (auNid()) {
            colonie->pretReproduction(this);
        } else {
            // Retourner au nid
        }
    } else {
        // Comportement Ouvriere
        actionOuvriere();
    }
}





// int main() {
//     Terrain t{10, 10};
//     Coord c{0, 1};
//     Colonie Col{&t, c};
//     cout << Col << endl;
//     return 0;
// }