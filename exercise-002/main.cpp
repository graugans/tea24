#include <fmt/chrono.h>
#include <fmt/format.h>

#include "module.h"

auto main(int argc, char** argv) -> int
{
    /**
     * The {fmt} lib is a cross platform library for printing and formatting text
     * it is much more convenient than std::cout and printf
     * More info at https://fmt.dev/latest/api.html
     */
    fmt::print("Hello, {}!\n", argv[0]);

    doSomething(42);
    int bar = 42;
    doSomething(bar);
    fmt::print("The value of bar is: {}\n", bar);

    doSomethingPtr(&bar);
    fmt::print("The value of bar after doSomethingPtr is: {}\n", bar);

    int* returnedPtr = doReturnAPtr();
    fmt::print("The value of returnedPtr content  after doReturnAPtr is: {}\n", *returnedPtr);
    doSomething(42);
    fmt::print("The value of returnedPtr content  after doReturnAPtr is: {}\n", *returnedPtr);

    doSomethingStatic();
    doSomethingStatic();
    doSomethingStatic();
    doSomethingStatic();

    fmt::print("Value of foo {} address of foo {}\n", foo, fmt::ptr(&foo));
    fmt::print("Value of foo2 {} address of foo2 {}\n", foo2, fmt::ptr(&foo2));
    fmt::print("Value of data {} address of data {}\n", data, fmt::ptr(&data));
    fmt::print("Value of data2 {} address of data2 {}\n", data2, fmt::ptr(&data2));
    fmt::print("Value of rodata {} address of rodata {}\n", rodata, fmt::ptr(&rodata));
    fmt::print("Value of rodata2 {} address of rodata2 {}\n", rodata2, fmt::ptr(&rodata2));

    int var;
    static int var2;
    fmt::print("Value of var {} address of var {}\n", var, fmt::ptr(&var));
    fmt::print("Value of var2 {} address of var2 {}\n", var2, fmt::ptr(&var2));

    fmt::print("Sizeof Big Data Blob {} address {}\n", sizeof(big_data_blob), fmt::ptr(big_data_blob));

    const int* array_ptr = big_data_blob;
    fmt::print("Sizeof Big Data Blob {} address {}\n", sizeof(array_ptr), fmt::ptr(array_ptr));

    fmt::print("array_ptr[0] {}\n", array_ptr[0]);
    fmt::print("big_data_blob[0] {}\n", big_data_blob[0]);

    return 0; /* exit gracefully*/
}
