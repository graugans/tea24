/*
 * Übung 004, Teil E: dasselbe Programm in mehreren Link-Varianten.
 *
 * main.c und greet.c in diesem Ordner gehören zu Teil A bis D und werden von Hand
 * mit gcc übersetzt. CMake baut nur diese Datei.
 */
#include <fmt/format.h>

#include "version/version.h"

int main()
{
    namespace version = tea::version;

    fmt::println("Project: {}", version::projectName());
    fmt::println("Version: {}.{}.{}", version::major(), version::minor(), version::patch());
    fmt::println("Full:    {}", version::fullVersion());

    return 0;
}
