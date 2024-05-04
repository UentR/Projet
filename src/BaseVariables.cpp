#include "BaseVariables.hpp"

map<string, float*> VARIABLES;

void SetupVar() {
    // Read Parametre.txt
    ifstream file("Parametre.txt");
    string temp;
    string name;
    float value;
    while (file >> temp) {
        if (temp == "#") {
            file >> name >> value;
            float* ptr = new float(value);
            VARIABLES[name] = ptr;
        }
    }
    float *PourcentF = new float[3]{*VARIABLES["PourcentO"], *VARIABLES["PourcentG"], *VARIABLES["PourcentR"]};
    float *NbF = new float[3]{*VARIABLES["NBO"], *VARIABLES["NBG"], *VARIABLES["NBR"]};
    VARIABLES["PourcentF"] = PourcentF;
    VARIABLES["NbF"] = NbF;
}