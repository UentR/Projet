#include "Coord.hpp"

using namespace std;


Coord::Coord(int a, int b) : ind_lig{a}, ind_col{b} {
    if (a > TAILLEGRILLE - 1  or a < 0 or b > TAILLEGRILLE - 1 or b < 0){
        throw runtime_error("Coord qui ne sont pas dans le tableau");
    }
}

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

ostream &operator<<(ostream &out , Coord coordo){
    out << coordo.getLigne() << " " << coordo.getColonne();
    return out;
}

bool operator==(Coord c1, Coord c2){
    return c1.getLigne() == c2.getLigne() and c1.getColonne() == c2.getColonne(); 
}

bool operator!=(Coord c1, Coord c2){
    return !(c1 == c2);
}

int main() {
    Coord c{1, 2};
    Coord c_{1,2};
    Coord not_c{2,1};
    cout<< c << endl;
    bool test = c == c_; 
    cout << test << " corr : true" << endl;
    test = c == not_c;
    cout << test << " corr : false" << endl;
    test = c != c_;
    cout << test << " corr : false" << endl;
    test = c != not_c;
    cout << test << " corr : true" << endl;
    return 0;
}