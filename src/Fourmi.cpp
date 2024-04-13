#include "Fourmi.hpp"
#include <iostream>
#include <string>

using namespace std;

// Constructeurs

Fourmi::Fourmi() {
    quantiteSucre = 0;
    isAttacked = false;
}

Guerriere::Guerriere(Coord c, int Idx) {
    vie = VieG;
    type = 1;
    position = c;
    colonie = Idx;
    quantiteSucre = 0;
    capaciteSucre = CapaciteSG;
    forceAttaque = AttaqueG;
}

Ouvriere::Ouvriere(Coord c, int Idx) {
    vie = VieO;
    type = 0;
    position = c;
    colonie = Idx;
    quantiteSucre = 0;
    capaciteSucre = CapaciteSO;
    forceAttaque = AttaqueO;
}

Reproductrice::Reproductrice(Coord c, int Idx) {
    vie = VieR;
    type = 2;
    position = c;
    colonie = Idx;
    quantiteSucre = 0;
    capaciteSucre = CapaciteSR;
    forceAttaque = AttaqueR;
}


Colonie::Colonie(Coord c) : nbFourmis{0}, typeFourmis{{0, 0, 0}}, quantiteSucre{0}, reproducticeEnAttente{0}, Idx{0} {
    position = c;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < NbF[i]; j++) {
            Fourmi f;
            if (i == 0) {
                Ouvriere o{c, Idx};
                f = o;
            } else if (i == 1) {
                Guerriere g{c, Idx};
                f = g;
            } else {
                Reproductrice r{c, Idx};
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

int Colonie::getIdx() const {
    return Idx;
}

int *Colonie::getPosition() const {
    return position;
}

ostream &operator<<(ostream &out, Fourmi f) {
    out << "Fourmi type : " << f.type << ", vie : " << f.vie << ", porte : " << f.quantiteSucre;
    return out;
}

ostream &operator<<(ostream &out, Colonie c) {
    out << "Colonie : " << c.getIdx() << ", nbFourmis : " << c.getNbFourmis() << ", position : " << c.getPosition() << ", quantiteSucre : " << c.quantiteSucre << endl;
    for (Fourmi fourmi : c.fourmis) {
        out << '\t' << fourmi << endl;
    }
    return out;
}

bool mettrePheromone() {}

int main() {
    Colonie c{{0, 1}};
    cout << c << endl;
    return 0;
}