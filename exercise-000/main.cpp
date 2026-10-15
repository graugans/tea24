#include <fmt/chrono.h>
#include <fmt/format.h>
#include <stdlib.h>

#include "config.h"

void changeValueByParameter(int value);
void changeValueByPointer(int* ptr);
void changeValueByReference(int& ref);

int main(int argc, char **argv) {

    int var = 4711;
    int x = 10;
    int* ptr = &x;

    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/12.0/api/
     */
    fmt::print("Hello, {}!\n", tea::PROJECT_NAME);
    fmt::print("Value of var: {}!\n", var);
    fmt::println("Value of argc: {}", argc);

    for(int i = 0; i < argc; i++) {
        fmt::println("The value of i: {} the content of: argc[{}]: {}, the address of argv[{}]: {}",i,i, argv[i],i,fmt::ptr(argv[i]));
    }
    fmt::println("The address of argv: {}",fmt::ptr(argv));

    fmt::println("The value of x: {} the address of x: {}", x, fmt::ptr(&x));
    fmt::println("The value ptr points at: {} the address of ptr: {}", *ptr, fmt::ptr(ptr));

    *ptr = 42;

    fmt::println("The value of x: {} the address of x: {}", x, fmt::ptr(&x));
    fmt::println("The value ptr points at: {} the address of ptr: {}", *ptr, fmt::ptr(ptr));


    double* d = (double*)malloc(sizeof(double));  // Speicher reservieren
    *d = 3.145;
    fmt::println("Value of d: {} address of d: {}", *d, fmt::ptr(d));
    free(d);
    fmt::println("Value of d: {} address of d: {}", *d, fmt::ptr(d)); // Fehler, use after free!!!
    d = new double(3.145);
    fmt::println("Value of d: {} address of d: {}", *d, fmt::ptr(d));
    delete d;
    fmt::println("Value of d: {} address of d: {}", *d, fmt::ptr(d)); // Fehler, use after free!!!

    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var));
    changeValueByParameter(var);
    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var));

    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var));
    changeValueByPointer(&var);
    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var));

    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var));
    changeValueByReference(var);
    fmt::println("[main] The value of var: {}, the address of var: {}", var, fmt::ptr(&var)); 

    return 0; /* read the value back with echo $? */
}

void changeValueByParameter(int value) {
    fmt::println("[changeValueByParameter] The value of 'value': {}",value);
    value += 10;
    fmt::println("[changeValueByParameter] The value of 'value': {}",value);
}
void changeValueByPointer(int* ptr) {
    fmt::println("[changeValueByPointer] The value of the address 'ptr' points at: {} the address of ptr {}",
        *ptr, 
        fmt::ptr(ptr)
    );
    *ptr += 10;
    fmt::println("[changeValueByPointer] The value of the address 'ptr' points at: {} the address of ptr {} ",
        *ptr, 
        fmt::ptr(ptr)
    );
}

void changeValueByReference(int& ref) {
    fmt::println("[changeValueByReference] The value of 'value': {}",ref);
    ref += 10;
    fmt::println("[changeValueByReference] The value of 'value': {}",ref);
}
