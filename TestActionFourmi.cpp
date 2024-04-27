#include <iostream>

using namespace std;

class Fourmi {
    public:
        int type;
        void ActionFourmi();

        Fourmi();
};

class Ouvriere : public Fourmi {
    public:
        void ActionFourmi();
        Ouvriere();
};

class Guerriere : public Fourmi {
    public:
        void ActionFourmi();
        Guerriere();
};

class Reproductrice : public Ouvriere {
    public:
        void ActionFourmi();
        Reproductrice();
};

void Fourmi::ActionFourmi() {
    cout << "Action generique" << endl;
}

Fourmi::Fourmi() {
    cout << "Constructeur Fourmi" << endl;
}

Ouvriere::Ouvriere() {
    type = 0;
    cout << "Constructeur Ouvriere" << endl;
}

Guerriere::Guerriere() {
    type = 1;
    cout << "Constructeur Guerriere" << endl;
}

Reproductrice::Reproductrice() {
    type = 2;
    cout << "Constructeur Reproductrice" << endl;
}


void Ouvriere::ActionFourmi() {
    cout << type << endl;
    cout << "Action ouvriere" << endl;
    Fourmi::ActionFourmi();
}

void Guerriere::ActionFourmi() {
    cout << "Action guerriere" << endl;
    Fourmi::ActionFourmi();
}

void Reproductrice::ActionFourmi() {
    cout << "Action reproductrice" << endl;
    // Call the parent method ActionFourmi
    Ouvriere::ActionFourmi();
}

int main() {
    Fourmi f;
    Ouvriere o;
    Guerriere g;
    Reproductrice r;

    f.ActionFourmi();
    o.ActionFourmi();
    g.ActionFourmi();
    r.ActionFourmi();

    return 0;
}