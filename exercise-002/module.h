#ifndef WORKSPACES_TEA24_EXERCISE_002_MODULE_H
#define WORKSPACES_TEA24_EXERCISE_002_MODULE_H

/* Includes gehören *innerhalb* des Include Guards */
#include "bonus_config.h"
#include "helper.h"

/* ---- Schnittstelle: Variablen mit externer Bindung (Definition in module.cpp) ---- */

/* .bss */
extern int foo;
extern int foo2;

/* .data */
extern int data;
extern int data2;

/* .rodata – das extern ist bei const in C++ Pflicht, sonst hätte die Definition
 * in module.cpp interne Bindung und main.cpp fände sie beim Linken nicht. */
extern const int rodata;
extern const int rodata2;
extern const int big_data_blob[2];

/*
Teil 2: Das hier ist NICHT möglich, weil moduleCounter in module.cpp static ist:

[build] /workspaces/tea24/exercise-002/module.cpp:21:12: error: static declaration of 'moduleCounter' follows non-static declaration
[build]    21 | static int moduleCounter;
[build]       |            ^
[build] /workspaces/tea24/exercise-002/module.h:17:12: note: previous declaration is here
[build]    17 | extern int moduleCounter;
[build]       |            ^

extern int moduleCounter;
*/

/* ---- Funktionen ---- */

void doSomething(int value);

void doSomethingPtr(int* ptr);

int* doReturnAPtr();

/* Teil 1 */
void doSomethingStatic();

/* Teil 2 */
void printModuleStaticValues();

/* Teil 2 – Antwort auf die Frage "Was passiert bei einem Pointer auf eine
 * modulstatische Variable?": Der *Name* ist privat, der *Speicher* nicht. */
int* getModuleCounterPtr();

/* Teil 3 */
void printTheBSSStuff();

/* Bonus 1 */
void printTheModuleShadow();

/* Bonus 4 */
void printBonusSegments();

#endif /* WORKSPACES_TEA24_EXERCISE_002_MODULE_H */
