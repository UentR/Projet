#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

#ifndef Done
#define Done
enum DebugLevel {
  NOTHING = 0,
  ERROR = 1,
  INFO = 2,
  WARNING = 3,
  INFO_DETAIL = 4,
  ALL_LOG = 5,
};
#endif

/**
 * @brief Ecrit un message log vers un fichier de debug.
 * @param string logInfo - Le message a écrire.
 */
void writeToDebugFile(string logInfo, int level);

/**
 * @brief Vide le fichier de debug.
 */
void flushDebug();

/**
 * @brief Ecrit un message log vers un fichier de stats.
 * @param string logInfo - Le message a écrire.
 */
void writeToStatsFile(string logInfo);