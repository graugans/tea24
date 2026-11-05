#include "module.h"

#include <fmt/format.h>

static int shadow = 42;

/* .bss Segment */
int foo;
int foo2 = 0;

/* .data Segment */
int data = 45;
int data2 = 43;

/* .rodata Segment */

const int rodata = 45;
const int rodata2 = 43;

const int big_data_blob[] = {42,0xff};


static int moduleCounter;
static int moduleValue = 123;

void doSomething(int value)
{
    value = value + 1;
    fmt::print("doSomething, value {}\n", value);
}

void doSomethingPtr(int* ptr)
{
    *ptr = *ptr + 1;
    fmt::print("doSomethingPtr, value {}\n", *ptr);
}

int* doReturnAPtr()
{
    int value = 4711;
    fmt::print("doReturnAPtr, value {}\n", value);
    /* 
     * This causes a warning and shall be avoided by all means
     * warning: address of stack memory associated with local variable 'value' returned [-Wreturn-stack-address]
     */
    return &value;
}

void doSomethingStatic()
{
    static int value = 56;
    int noneStatic = 42;
    static bool once = false;

    fmt::println(
        "[{}] doSomethingStatic, value: {}, address of value: {}",
         __func__, 
         value, 
         fmt::ptr(&value)
        );
    fmt::println(
        "[{}] doSomethingStatic, noneStatic: {}, address of noneStatic: {}",
         __func__, 
         noneStatic, 
         fmt::ptr(&noneStatic)
        );

    /* increment the variables by one */
    value = value + 1;
    noneStatic = noneStatic + 1;
    
    /* Let's call ourself to show the stack frame ... */
    if(!once) {
        once = true;
        doSomethingStatic();
    }
}

void printModuleStaticValues() {
    moduleCounter = moduleCounter + 1;
    fmt::println(
        "[{}], moduleCounter: {}, address of moduleCounter: {}",
         __func__, 
         moduleCounter, 
         fmt::ptr(&moduleCounter)
        );

    fmt::println(
        "[{}], moduleValue: {}, address of moduleValue: {}",
         __func__, 
         moduleValue, 
         fmt::ptr(&moduleValue)
        );
    fmt::println(
        "[{}], data(.data): {}, address of data: {}",
         __func__, 
         data, 
         fmt::ptr(&data)
        );
    moduleValue = moduleValue + 42;
}

void printTheBSSStuff() {
    fmt::println(
        "[{}], foo(.bss): {}, address of foo: {}",
         __func__, 
         foo, 
         fmt::ptr(&foo)
        );
    fmt::println(
        "[{}], foo2(.bss): {}, address of foo: {}",
         __func__, 
         foo2, 
         fmt::ptr(&foo2)
        );

}

void printTheModuleShadow() {
    fmt::println(
        "[{}], shadow: {}, address of shadow: {}",
         __func__, 
         shadow, 
         fmt::ptr(&shadow)
        );

}