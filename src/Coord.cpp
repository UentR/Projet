#include "Coord.hpp"

using namespace std;


Coord::Coord(int a, int b) : ind_lig{a}, ind_col{b} {
    if (a > TAILLEGRILLE - 1  or a < 0 or b > TAILLEGRILLE - 1 or b < 0){
        throw runtime_error("Coord qui ne sont pas dans le tableau");
    }
}

Coord::Coord() : ind_lig{0}, ind_col{0} {}

int Coord::getLigne() const {
    return ind_lig;
}

int Coord::getColonne() const {
    return ind_col;
}


bool Coord::setCoord(int a, int b){
    if (a > TAILLEGRILLE - 1  or a < 0 or b > TAILLEGRILLE - 1 or b < 0){
        throw runtime_error("Coord qui ne sont pas dans le tableau");
        return false;
    }
    ind_col = a;
    ind_lig = b;
    
    return true; 
}
ostream &operator<<(ostream &out , Coord coordo) {
    out << coordo.getLigne() << " " << coordo.getColonne();
    return out;
}

bool operator==(Coord c1, Coord c2){
    return c1.getLigne() == c2.getLigne() and c1.getColonne() == c2.getColonne(); 
}

bool operator!=(Coord c1, Coord c2){
    return !(c1 == c2);
}


ostream &operator<<(ostream &out; EnsembleCoord tab){
    for (int i=0; i<tab.Grille.size();i+++){
        out << tab[i] << " " ;
    }
    out << endl;
    return out;
}

int EnsembleCoord::position(Coord objet) const{
    for (int i=0;i<Grille.size();i++){
        if (Grille[i] == objet){
            return i;
        }
    }
    return -1;
}

bool EnsembleCoord::contient(Coord objet) const{
    if (position(objet) != -1){
        return true;
    }
    return false;
}

void EnsembleCoord::ajoute(Coord objet){
    if (!contient(objet)){
        Grille.push_back(objet);
    }
}

void EnsembleCoord::supprime(Coord objet){
    if (contient(objet)){
        Grille.pop_back(objet);
    }else{
        throw invalid_argument("Cette coordonnee n'est pas dans la grille");
    }
}

bool EnsembleCoord::estVide() const{
    return Grille.size() == 0;
}

int taille()const{
    return Grille.size();
}

Coord EnsembleCoord::ieme(int n) const{
    return Grille[n];
}

// int main() {
//     Coord c{1, 2};
//     Coord c_{1,2};
//     Coord not_c{2,1};
//     cout<< c << endl;
//     bool test = c == c_; 
//     cout << test << " corr : true" << endl;
//     test = c == not_c;
//     cout << test << " corr : false" << endl;
//     test = c != c_;
//     cout << test << " corr : false" << endl;
//     test = c != not_c;
//     cout << test << " corr : true" << endl;
//     return 0;
// }
