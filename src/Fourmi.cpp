#include "Fourmi.hpp"
#include <iostream>
#include <string>

using namespace std;

// cout

ostream &operator<<(ostream &out, Fourmi f) {
    out << "Fourmi type : " << f.type << ", vie : " << f.vie << ", porte : " << f.quantiteSucre;
    return out;
}

ostream &operator<<(ostream &out, Colonie c) { // ", position : " << c.getPosition() <<
    Coord coordo = c.getPosition();
    out << "Colonie : " << c.getIdx() << ", nbFourmis : " << c.getNbFourmis()
        << ", position : " << c.getPosition() << ", quantiteSucre : "
        << c.quantiteSucre << endl;
    for (Fourmi fourmi : c.fourmis) {
        out << '\t' << fourmi << endl;
    }
    return out;
}


// Constructeurs

Fourmi::Fourmi() : colonieIdx{-1}, position{0, 0}, vision{1} {
    quantiteSucre = 0;
    isAttacked = false;
    colonie = nullptr;
}

Fourmi::Fourmi(Coord c, int Idx, Colonie *c) : colonieIdx{Idx}, position{c}, colonie{c}, vision{1} {
    quantiteSucre = 0;
    isAttacked = false;
}

Ouvriere::Ouvriere(Coord c, int Idx) : Fourmi(c, Idx) {
    vie = VieO;
    type = 0;
    capaciteSucre = CapaciteSO;
    forceAttaque = AttaqueO;
}

Guerriere::Guerriere(Coord c, int Idx) : Fourmi(c, Idx) {
    vie = VieG;
    type = 1;
    capaciteSucre = CapaciteSG;
    forceAttaque = AttaqueG;
}

Reproductrice::Reproductrice(Coord c, int Idx) : Fourmi(c, Idx) {
    vie = VieR;
    type = 2;
    capaciteSucre = CapaciteSR;
    forceAttaque = AttaqueR;
}

Colonie::Colonie(Coord c) : nbFourmis{0}, typeFourmis{{0, 0, 0}}, quantiteSucre{0}, reproducticeEnAttente{0}, Idx{0}, position{c} {
    Fourmi f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NbF[i]; j++) {
            if (i == 0) {
                Ouvriere o{c, Idx, this};
                f = o;
            } else if (i == 1) {
                Guerriere g{c, Idx, this};
                f = g;
            } else {
                Reproductrice r{c, Idx, this};
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
                Ouvriere o{position, Idx};
                fourmis.push_back(o);
            } else if (comp == 1) {
                Guerriere g{position, Idx};
                fourmis.push_back(g);
            } else {
                Reproductrice r{position, Idx};
                fourmis.push_back(r);
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

Coord Colonie::getPosition() const {
    return position;
}

void Colonie::deleteFourmi(Fourmi f) {
    int i = 0;
    for (Fourmi fourmi : fourmis) {
        if (fourmi == f) {
            fourmis.erase(fourmis.begin()+i);
            nbFourmis--;
            typeFourmis[fourmi.type]--;
            return;
        }
        i++;
    }
}

void Colonie::pretReproduction(Fourmi *f) {
    reproducticeEnAttente.insert(f);
}

bool Colonie::CheckReproduction() {
    if (reproducticeEnAttente.size() >= 2) {
        return true;
    }
    return false;
}

// Methodes fourmis

void Fourmi::mettrePheromone(Terrain t) const {
    t.Cells[t.toIdx(position.x, position.y)].pheromones[colonie] = PosePheromones;
}

float Fourmi::attaquer(Fourmi *f) {
    f->isAttacked = true;
    vie -= f->forceAttaque;
    f->vie -= forceAttaque;
    return f->vie;
}

int Fourmi::ramasserSucre(Terrain t, Coord c) {
    int Idx = t.toIdx(c.getColonne(), c.getLigne());
    if (t.Cells[Idx].containsSugar()) {
        int sugar = t.Cells[Idx].removeSugar(capaciteSucre);
        quantiteSucre += sugar;
        return sugar;
    }
    return 0;
}

bool Fourmi::deposerSucre(Terrain t, Coord c) {
    int Idx = t.toIdx(c.getColonne(), c.getLigne());
    if (t.Cells[Idx].containsNest(colonieIdx)) {
        t.Cells[Idx].sugarAmount += quantiteSucre;
        quantiteSucre = 0;
        return true;
    }
    return false;
}

bool Fourmi::deplacer(Terrain t, Coord c) {
    int Idx = t.toIdx(c.getColonne(), c.getLigne());
    if (t.Cells[Idx].isEmpty()) {
        position = c;
        return true;
    }
    return false;
}

vector<Fourmi *> Fourmi::fourmiProche() const {
    vector<Fourmi *> fourmisProches;
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        for (Fourmi *f : c->toAnt) {
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
        if (c.getState() == State) {
            return c;
        }
    }
    return Coord{-1, -1};
}

vector<Coord> Fourmi::pheromonesProches(int Idx) const {
    vector<Coord> pheromonesProches;
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        if (c.containsPheromone(Idx)) {
            pheromonesProches.push_back(c);
        }
    }
    return pheromonesProches;
}

void Fourmi::choixAction(Terrain t) {
    if (quantiteSucre>MinSucre) {
        if (auNid()) { 
            // Déposer sucre
            deposerSucre(t, colonie->getPosition());
        }
        else {
            // se déplacer vers le nid
        }
    } else {
        Coord Sucre = procheState(1);
        if (Sucre != {-1, -1}) {
            // Ramasser sucre
            ramasserSucre(t, Sucre);
            mettrePheromone(t);
        } 
        else {
            vector<Coord> pheromones = pheromonesProches(vision, colonieIdx);
            if (pheromones.size() > 0) { 
                // Suivre pheromone
                deplacer(t, pheromones[0]);
            } else {    
                // Se déplacer aléatoirement
                vector<Coord> voisin = procheState(vision, 2);
                deplacer(t, voisin[0]);
            }
        }
    }

}


void Guerriere::actionGuerriere(Terrain t) {
    vector<Fourmi *> fourmisProches = fourmiProche();
    if (fourmisProches.size() > 0) {
        // attaquer
        attaquer(fourmisProches[0]);
    } else {
        vector<Coord> pheromones = pheromonesProches(-colonieIdx);
        if (pheromones.size() > 0) {
            // Suivre autre colonie
            deplacer(t, pheromones[0]);
        } else {
            // Comportement normal
            choixAction(t);
        }
    }
}

void Ouvriere::fuir() {
    vector<Coord> voisins = position.voisin(vision);
    for (Coord c : voisins) {
        if (c.isEmpty()) {
            deplacer(t, c);
            return;
        }
    }
}


void Ouvriere::actionOuvriere(Terrain t) {
    if (isAttacked) {
        // Fuir
        fuir();
    } else {
        // Comportement normal
        choixAction(t);
    }
}

bool Reproductrice::checkReproduction() {
    if (colonie->quantiteSucre > MinSucre) {
        return true;
    }
    return false;
}



void Reproductrice::actionReproductrice(Terrain t) {
    if (checkReproduction()) {
        // Reproduire
        if (auNid()) {
            colonie.preteReproduction(this);
        } else {
            // Retourner au nid
        }
    } else {
        // Comportement Ouvriere
        actionOuvriere(t);
    }
}

int main() {
    Coord c{0, 1};
    Colonie Col{c};
    cout << Col << endl;
    return 0;
}