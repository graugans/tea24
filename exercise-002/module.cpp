#include "module.h"

#include <fmt/format.h>

/* Bonus 2: absichtlich ein zweites Mal – zusätzlich zum Include über module.h.
 * Ohne Include Guard in bonus_config.h => "redefinition of 'BONUS_MAGIC'" */
#include "bonus_config.h"

/* =====================================================================
 * Variablen mit EXTERNER Bindung (nm: Großbuchstabe B / D / R)
 * ===================================================================== */

/* .bss – Wert 0, belegt in der Datei keinen Platz, nur im RAM */
int foo;       // ohne Initialisierer -> automatisch 0
int foo2 = 0;  // explizit 0 -> landet trotzdem in .bss!

int bss;
int bss2;

/* .data – Wert != 0, muss in der Binärdatei (bzw. im Flash) gespeichert werden */
int data = 45;
int data2 = 43;

/* .rodata – schreibgeschützt */
const int rodata = 45;
const int rodata2 = 43;

const int big_data_blob[] = {42, 0xff};

/* =====================================================================
 * Teil 2 + 3: Modul-lokale Variablen, INTERNE Bindung
 * (nm: Kleinbuchstabe b / d / r) – nicht in module.h deklariert!
 * ===================================================================== */
static int moduleCounter;                         // .bss
static int moduleValue = 123;                     // .data
static const int moduleConst = 77;                // .rodata – weil wir die Adresse ausgeben
[[maybe_unused]] static const int moduleConstUnused = 78;  // gar kein Symbol: nie benutzt -> wegoptimiert

/* Bonus 1: gleicher Name wie die globale Variable in main.cpp,
 * aber dank static ein komplett eigenes Objekt */
static int shadow = 42;  // .data, nm: d

/* Bonus 4: Null- vs. Nicht-Null-Initialisierung */
namespace bonus4
{
int a;            // .bss
int b = 0;        // .bss
int c = 1;        // .data
const int d = 1;  // .rodata – const auf Namespace-Ebene hat in C++ automatisch interne Bindung (nm: r)
}  // namespace bonus4

/* =====================================================================
 * Funktionen aus der Vorlage
 * ===================================================================== */

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

/* =====================================================================
 * Teil 1: static innerhalb einer Funktion
 * ===================================================================== */

void doSomethingStatic()
{
    static int value = 56;     // genau EINMAL initialisiert, lebt so lange wie das Programm -> .data
    int nonStatic = 42;        // bei JEDEM Aufruf neu angelegt und initialisiert -> Stack
    static bool once = false;  // = 0 -> .bss

    tea::printVar(__func__, "value (static)", ".data", value);
    tea::printVar(__func__, "nonStatic", "stack", nonStatic);

    /* increment the variables by one */
    value = value + 1;
    nonStatic = nonStatic + 1;  // wirkungslos: die Variable stirbt am Ende des Aufrufs

    /* Einmal uns selbst aufrufen, um einen zweiten Stack Frame zu sehen ... */
    if (!once) {
        once = true;
        tea::printVar(__func__, "once (static)", ".bss", once);
        fmt::println("[{:<24}] -> rekursiver Aufruf, neuer Stack Frame", __func__);
        doSomethingStatic();
    }
}

/* =====================================================================
 * Teil 2: static auf Modulebene
 * ===================================================================== */

void printModuleStaticValues()
{
    moduleCounter = moduleCounter + 1;

    tea::printVar(__func__, "moduleCounter (static)", ".bss", moduleCounter);
    tea::printVar(__func__, "moduleValue (static)", ".data", moduleValue);
    tea::printVar(__func__, "data", ".data", data);

    moduleValue = moduleValue + 42;
}

int* getModuleCounterPtr()
{
    /* Völlig legal: static schränkt nur die Sichtbarkeit des NAMENS ein.
     * Wer die Adresse bekommt, kann die Variable lesen und schreiben. */
    return &moduleCounter;
}

/* =====================================================================
 * Teil 3: Segmente
 * ===================================================================== */

void printTheBSSStuff()
{
    tea::printVar(__func__, "foo", ".bss", foo);
    tea::printVar(__func__, "foo2 (= 0)", ".bss", foo2);
    tea::printVar(__func__, "moduleConst (static)", ".rodata", moduleConst);
}

/* =====================================================================
 * Bonus 1: Schatten-Variable
 * ===================================================================== */

void printTheModuleShadow()
{
    tea::printVar(__func__, "shadow (static, module)", ".data", shadow);
}

/* =====================================================================
 * Bonus 4: Segmentanalyse
 * ===================================================================== */

void printBonusSegments()
{
    tea::printVar(__func__, "bonus4::a", ".bss", bonus4::a);
    tea::printVar(__func__, "bonus4::b (= 0)", ".bss", bonus4::b);
    tea::printVar(__func__, "bonus4::c (= 1)", ".data", bonus4::c);
    tea::printVar(__func__, "bonus4::d (const)", ".rodata", bonus4::d);
    fmt::println("[{:<24}] BONUS_MAGIC aus bonus_config.h = {}", __func__, BONUS_MAGIC);
}
