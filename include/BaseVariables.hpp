#ifndef FOURMI_HPP

#include <array>

using namespace std;

#define FOURMI_HPP

#define NbTourAttaque 5

// Vie des fourmis
#define VieO 10
#define VieG 15
#define VieR 10

// Attaque des fourmis
#define AttaqueO 3
#define AttaqueG 6
#define AttaqueR 2

// Force de portée des fourmis
#define CapaciteSO 5
#define CapaciteSG 10
#define CapaciteSR 5

// Ratio de fourmis
#define PourcentO 0.75
#define PourcentG 0.15
#define PourcentR 0.10


extern array<float, 3> PourcentF;

// Nombre initial de fourmis
#define NBO 15
#define NBG 3
#define NBR 2

extern array<int, 3> NbF;

#define PosePheromones 255
#define MinSucre 0

#define SQUARESIZE 1

#define SPAWNRADIUS 3

#endif