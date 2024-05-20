#include <iostream>
#include <typeinfo>

#include <vector>

using namespace std;

class Fourmi {
    public:
        Fourmi() {}
        virtual void choix() { cout << "Fourmi" << endl; }
};

class Ouvriere : public Fourmi {
    public:
        Ouvriere() { A = 0; }
        int A;
        void choix() { cout << "Ouvriere" << endl; }
};


int main() {
    Ouvriere f{};
    f.choix();
    // Show type of f
    cout << typeid(f).name() << endl;

    vector<Fourmi *> fourmis = {&f};

    for (Fourmi *f : fourmis) {
        f->choix();
        cout << typeid(f).name() << endl;
    }
    return 0;
}