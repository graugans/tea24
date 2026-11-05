#include <stdint.h>

#ifndef WORKSPACES_TEA22_EXERCISE_003_MODULE_H
#define WORKSPACES_TEA22_EXERCISE_003_MODULE_H

extern int foo;
extern int foo2;
extern int data;
extern int data2;
extern const int rodata;
extern const int rodata2;
extern const int big_data_blob[2];

/*
This is not possible due the fact moduleCounter is static

[build] /workspaces/tea24/exercise-002/module.cpp:21:12: error: static declaration of 'moduleCounter' follows non-static declaration
[build]    21 | static int moduleCounter;
[build]       |            ^
[build] /workspaces/tea24/exercise-002/module.h:17:12: note: previous declaration is here
[build]    17 | extern int moduleCounter;
[build]       |            ^

extern int moduleCounter;
*/

void doSomething(int value);

void doSomethingPtr(int* ptr);

int* doReturnAPtr();

void doSomethingStatic();

void printModuleStaticValues();

void printTheBSSStuff();

void printTheModuleShadow();
#endif