/*
 * Bonus 3: Die C-API ist ein dünner Wrapper um die C++-API.
 * Die Datei wird mit dem C++-Compiler übersetzt, die Funktionen bekommen aber C-Linkage,
 * weil version_c_api.h sie in extern "C" deklariert.
 */
#include "version/version_c_api.h"

#include <string>

#include "version/version.h"

const char* version_full_c(void)
{
    /* Funktionslokale static-Variable (vgl. Übung 002!):
     *  - wird beim ERSTEN Aufruf genau einmal initialisiert – thread-sicher seit C++11
     *    (der Compiler legt dafür eine Guard-Variable an: nm zeigt _ZGVZ14version_full_cE4full)
     *  - lebt bis Programmende -> der zurückgegebene Zeiger bleibt gültig
     * Eine lokale (nicht-static) std::string wäre hier ein Dangling Pointer! */
    static const std::string full = tea::version::fullVersion();
    return full.c_str();
}

const char* version_name_c(void)
{
    return tea::version::projectName();
}

int version_major_c(void)
{
    return tea::version::major();
}

int version_minor_c(void)
{
    return tea::version::minor();
}

int version_patch_c(void)
{
    return tea::version::patch();
}

int version_is_at_least_c(int major, int minor, int patch)
{
    return tea::version::isAtLeast(major, minor, patch) ? 1 : 0;
}
