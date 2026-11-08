#include <fmt/format.h>

#include "config.h"
#include "module.h"

/* Bonus 1: externe Bindung – gleicher Name wie das `static int shadow` in module.cpp.
 * Kein Linkerfehler, weil das static-shadow den Linker nie erreicht. */
int shadow = 11;

auto main() -> int
{
    fmt::println("{} {} – static, Sichtbarkeit & Speichersegmente", tea::PROJECT_NAME, tea::PROJECT_VER);
    fmt::println("Programm geladen ab {} (ASLR: bei jedem Start anders!)", fmt::ptr(__executable_start));

    /* ------------------------------------------------------------------ */
    tea::printHeading("Teil 1: static innerhalb einer Funktion");
    doSomethingStatic();  // 1. Aufruf: ruft sich einmal selbst auf
    doSomethingStatic();  // 2. Aufruf: value zählt weiter, nonStatic beginnt wieder bei 42

    /* ------------------------------------------------------------------ */
    tea::printHeading("Teil 2: static auf Modulebene");
    printModuleStaticValues();
    printModuleStaticValues();

    /* moduleCounter = 1000;   // error: use of undeclared identifier 'moduleCounter'  */
    int* counter = getModuleCounterPtr();
    *counter = 1000;
    fmt::println("[{:<24}] *getModuleCounterPtr() = 1000  -> Name privat, Speicher nicht!", __func__);
    printModuleStaticValues();

    /* ------------------------------------------------------------------ */
    tea::printHeading("Teil 3: In welchem Segment landen die Variablen?");
    printTheBSSStuff();

    /* dieselben Objekte aus einer anderen Übersetzungseinheit – gleiche Adressen */
    tea::printVar(__func__, "foo", ".bss", foo);
    tea::printVar(__func__, "foo2", ".bss", foo2);
    tea::printVar(__func__, "bss", ".bss", bss);
    tea::printVar(__func__, "bss2", ".bss", bss2);
    tea::printVar(__func__, "data", ".data", data);
    tea::printVar(__func__, "data2", ".data", data2);
    tea::printVar(__func__, "rodata", ".rodata", rodata);
    tea::printVar(__func__, "rodata2", ".rodata", rodata2);
    tea::printVar(__func__, "big_data_blob[0]", ".rodata", big_data_blob[0]);

    fmt::println("\nSegmentgrenzen laut Linker (nm-Offset):");
    fmt::println("  .text   endet bei   {:#06x}  (etext)", tea::imageOffset(etext));
    fmt::println("  .data   beginnt bei {:#06x}  (__data_start)", tea::imageOffset(__data_start));
    fmt::println("  .bss    beginnt bei {:#06x}  (edata)", tea::imageOffset(edata));
    fmt::println("  .bss    endet bei   {:#06x}  (end)", tea::imageOffset(end));

    /* ------------------------------------------------------------------ */
    tea::printHeading("Bonus 1: gleichnamige Variablen in main.cpp und module.cpp");
    tea::printVar(__func__, "shadow (global, main)", ".data", shadow);
    printTheModuleShadow();

    /* ------------------------------------------------------------------ */
    tea::printHeading("Bonus 4: Null- und Nicht-Null-Initialisierung");
    printBonusSegments();

    return 0; /* exit gracefully*/
}
