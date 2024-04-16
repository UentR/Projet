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

Fourmi::Fourmi() : colonieIdx{-1}, position{0, 0} {
    quantiteSucre = 0;
    isAttacked = false;
    colonie = nullptr;
}

Fourmi::Fourmi(Coord c, int Idx, Colonie *c) : colonieIdx{Idx}, position{c}, colonie{c} {
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
    int comp = 0 ;
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

// Methodes fourmis

void Fourmi::mettrePheromone(Terrain t) const {
    t.Cells[t.toIdx(position.x, position.y)].pheromones[colonie] = PosePheromones;
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



int main() {
    Coord c{0, 1};
    Colonie Col{c};
    cout << Col << endl;
    return 0;
}