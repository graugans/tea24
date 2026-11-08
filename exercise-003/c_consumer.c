/*
 * Bonus 3: Ein echtes C-Programm (übersetzt mit dem C-Compiler!) nutzt die C++-Bibliothek.
 *
 * Das geht nur, weil version_c_api.h die Funktionen mit extern "C" deklariert:
 * Der C-Compiler erzeugt Referenzen auf "version_full_c" – und genau so heißt das Symbol
 * auch in libversion_library.a, obwohl es von einem C++-Compiler stammt.
 *
 * Die C++-API (version/version.h) könnte hier NICHT eingebunden werden:
 * namespace, std::string und bool-Rückgaben gibt es in C nicht.
 */
#include <stdio.h>

#include "version/version_c_api.h"

int main(void)
{
    printf("[C] Name:    %s\n", version_name_c());
    printf("[C] Version: %d.%d.%d\n", version_major_c(), version_minor_c(), version_patch_c());
    printf("[C] Full:    %s\n", version_full_c());
    printf("[C] >= 0.1.0? %s\n", version_is_at_least_c(0, 1, 0) ? "ja" : "nein");
    printf("[C] >= 1.0.0? %s\n", version_is_at_least_c(1, 0, 0) ? "ja" : "nein");
    return 0;
}
