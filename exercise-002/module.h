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

void doSomething(int value);

void doSomethingPtr(int* ptr);

int* doReturnAPtr();

void doSomethingStatic();



#endif