#ifndef WORKSPACES_TEA24_EXERCISE_002_HELPER_H
#define WORKSPACES_TEA24_EXERCISE_002_HELPER_H

/*
 * helper.h – kleine Werkzeugkiste für Übung 002
 *
 * 1. extern-Deklarationen für bss/bss2 (Definition in module.cpp)
 * 2. tea::printVar(): gibt Name, Wert, Adresse UND das tatsächliche Segment
 *    einer Variablen aus – und prüft, ob unsere Erwartung stimmt.
 *
 * Wie findet das Programm zur Laufzeit heraus, in welchem Segment eine
 * Variable liegt? Der GNU-Linker (ld) stellt über sein Standard-Linkerskript
 * ein paar "magische" Symbole bereit, die selbst keinen Speicher belegen,
 * sondern nur Grenzen markieren (siehe `man 3 end`):
 *
 *   __executable_start  Anfang des Programms (Basisadresse, wegen ASLR variabel)
 *   etext               Ende von .text (Code)
 *   __data_start        Anfang von .data
 *   edata               Ende von .data   (== __bss_start)
 *   end                 Ende von .bss
 *
 * Zwischen etext und __data_start liegen .rodata, .eh_frame, .got, ...
 * -> alles schreibgeschützt; wir nennen es vereinfacht ".rodata".
 *
 * Hinweis: Das ist Linux/glibc/GNU-ld-spezifisch – auf einem Mikrocontroller
 * heißen die Symbole anders (z. B. _sdata/_edata/_sbss/_ebss im Linkerskript),
 * das Prinzip ist aber genau dasselbe.
 */

#include <fmt/format.h>

#include <cstdint>
#include <string_view>

extern int bss;
extern int bss2;

/* Vom Linker bereitgestellte Segmentgrenzen (C-Linkage, kein Name-Mangling) */
extern "C" const char __executable_start[];
extern "C" const char etext[];
extern "C" const char __data_start[];
extern "C" const char edata[];
extern "C" const char end[];

namespace tea
{

/** Ermittelt anhand der Linker-Symbole, in welchem Segment eine Adresse liegt. */
inline auto segmentOf(const void* address) -> std::string_view
{
    const auto addr = reinterpret_cast<std::uintptr_t>(address);
    const auto at = [](const char* symbol) { return reinterpret_cast<std::uintptr_t>(symbol); };

    if (addr < at(__executable_start) || addr >= at(end)) {
        return "stack";  // oder Heap / Shared Library – jedenfalls nicht im ELF-Image
    }
    if (addr < at(etext)) {
        return ".text";
    }
    if (addr < at(__data_start)) {
        return ".rodata";
    }
    if (addr < at(edata)) {
        return ".data";
    }
    return ".bss";
}

/**
 * Offset relativ zum Programmanfang. Genau diese Zahl zeigt `nm -S` an!
 * So lassen sich Laufzeitadressen (mit ASLR jedes Mal anders) direkt mit
 * der Symboltabelle der Binärdatei vergleichen.
 */
inline auto imageOffset(const void* address) -> std::uintptr_t
{
    return reinterpret_cast<std::uintptr_t>(address) - reinterpret_cast<std::uintptr_t>(__executable_start);
}

/**
 * Einheitliche Ausgabe einer Variablen:
 *
 *   [Funktion] Name  = Wert  @ Adresse  Segment (nm-Offset)  Erwartung OK?
 */
template <typename T>
void printVar(std::string_view where, std::string_view name, std::string_view expected, const T& var)
{
    const auto segment = segmentOf(&var);
    const auto verdict = (segment == expected) ? "✓" : fmt::format("✗ erwartet {}", expected);

    if (segment == "stack") {
        fmt::println("[{:<24}] {:<24} = {:>5}  @ {:>14}  {:<7}            {}", where, name, var, fmt::ptr(&var),
                     segment, verdict);
    } else {
        fmt::println("[{:<24}] {:<24} = {:>5}  @ {:>14}  {:<7} (nm {:#06x})  {}", where, name, var, fmt::ptr(&var),
                     segment, imageOffset(&var), verdict);
    }
}

/** Überschrift für die einzelnen Teile des Walkthroughs */
inline void printHeading(std::string_view title)
{
    fmt::println("\n── {} {:─<{}}", title, "", 100 - title.size());
}

}  // namespace tea

#endif /* WORKSPACES_TEA24_EXERCISE_002_HELPER_H */
