#include <fmt/chrono.h>
#include <fmt/format.h>

#include "config.h"

int bss;
int bss2 = 0;

int data = 4711;
double data2 = 0.815;

const int rodata = 42;
const double rodata2 = 42.3;

int returnAStaticValue();
int* returnAPointer();

int main(int argc, char **argv) {

    static int whoami = 32;
    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::print("Hello, {}!\n", tea::PROJECT_NAME);

    fmt::println("The value of bss: {} the address of bss {}", bss, fmt::ptr(&bss));
    fmt::println("The value of bss2: {} the address of bss2 {}", bss2, fmt::ptr(&bss2));

    fmt::println("The value of data: {} the address of data {}", data, fmt::ptr(&data));
    fmt::println("The value of data2: {} the address of data2 {}", data2, fmt::ptr(&data2));

    fmt::println("The value of rodata: {} the address of rodata {}", rodata, fmt::ptr(&rodata));
    fmt::println("The value of rodata2: {} the address of rodata2 {}", rodata2, fmt::ptr(&rodata2));
   
    fmt::println("[{}] The value of whoami: {} the address of whoami: {} ",
        __func__,
         whoami,
         fmt::ptr(&whoami)
        );
    
    fmt::println("The return value of (returnAStaticValue): {}", returnAStaticValue());
    fmt::println("Call it twice");
    fmt::println("The return value of (returnAStaticValue): {}", returnAStaticValue());
    int*  ptr = returnAPointer();
    fmt::println("The return value of (returnAPointer): {} the address value of the pointer: {}",
         *ptr, 
         fmt::ptr(ptr)
        );

    double *heap = new double(47.11);

    fmt::println("The value of heap: {} the address of heap:  {}", *heap, fmt::ptr(heap));
    return 0; /* exit gracefully*/
}

int returnAStaticValue() {
    static int whoami = 78;
    fmt::println("[{}] The value of whoami: {} the address of whoami: {} ",
        __func__,
         whoami,
         fmt::ptr(&whoami)
        );
    whoami += 22;
    return whoami++;
}

int* returnAPointer() {
    int pointerMe = 45;
    fmt::println("Value of pointerMe: {} address of pointerME {}", pointerMe, fmt::ptr(&pointerMe));
    return &pointerMe;
}