/*
 * Übung 003: Statische Bibliothek für Projekt-Versionierung – Walkthrough
 *
 * Diese Datei wird ZWEIMAL gebaut (siehe CMakeLists.txt):
 *   exercise-003         -> gegen libversion_library.a  (statisch)
 *   exercise-003-shared  -> gegen libversion_library.so (dynamisch, Bonus 4)
 * Der Quelltext ist identisch – nur das Linken unterscheidet sich.
 */
#include <dlfcn.h>  // dladdr(): "In welcher Datei liegt diese Funktion?" (POSIX/glibc)
#include <fmt/format.h>

#include <cstdio>
#include <string_view>

#include "config.h"                  // generiert: Konfiguration des PROGRAMMS
#include "version/version.h"         // Bibliothek: C++-API
#include "version/version_c_api.h"  // Bibliothek: C-API (Bonus 3)

#ifndef TEA_LINKAGE
#define TEA_LINKAGE "unbekannt"
#endif

namespace
{

void printHeading(std::string_view title)
{
    fmt::println("\n── {} {:─<{}}", title, "", 90 - title.size());
}

/** Fragt den dynamischen Loader, aus welcher Datei (Programm oder .so) eine Funktion stammt. */
void printOrigin(std::string_view name, const void* address)
{
    Dl_info info{};
    if (dladdr(address, &info) != 0 && info.dli_fname != nullptr) {
        fmt::println("  {:<34} @ {:>14}  aus {}", name, address, info.dli_fname);
    } else {
        fmt::println("  {:<34} @ {:>14}  (unbekannt)", name, address);
    }
}

template <typename Function>
auto addressOf(Function* function) -> const void*
{
    /* Funktionszeiger -> void*: unter POSIX erlaubt (dlsym/dladdr arbeiten genau so) */
    return reinterpret_cast<const void*>(function);
}

}  // namespace

auto main() -> int
{
    namespace version = tea::version;

    /* ------------------------------------------------------------------ */
    printHeading("Teil 4: Versionsinfos aus der Bibliothek");
    fmt::println("Project: {}", version::projectName());
    fmt::println("Version: {}.{}.{}", version::major(), version::minor(), version::patch());
    fmt::println("Full:    {}", version::fullVersion());

    /* ------------------------------------------------------------------ */
    printHeading("Wessen Version ist das? Drei CMake-Projekte, zwei Konfig-Header");
    fmt::println("  {:<42} {:<16} {:<8} {}", "Quelle", "Name", "Version", "configure_file() am");
    fmt::println("  {:<42} {:<16} {:<8} {}", "Root-CMakeLists.txt (CMAKE_PROJECT_NAME)", tea::TOP_LEVEL_PROJECT, "–",
                 "–");
    fmt::println("  {:<42} {:<16} {:<8} {}", "exercise-003/config.h", tea::PROJECT_NAME, tea::PROJECT_VER,
                 tea::PROJECT_BUILD_DATE);
    fmt::println("  {:<42} {:<16} {:<8} {}", "version_config.h (über die Lib-API)", version::projectName(),
                 fmt::format("{}.{}.{}", version::major(), version::minor(), version::patch()),
                 version::configureDate());

    /* ------------------------------------------------------------------ */
    printHeading("Bonus 1: isAtLeast() – lexikografischer Vergleich");
    struct Probe
    {
        int major, minor, patch;
    };
    for (const auto& [M, m, p] : {Probe{0, 0, 0}, Probe{0, 1, 1}, Probe{0, 1, 2}, Probe{0, 1, 3}, Probe{0, 2, 0},
                                  Probe{1, 0, 0}}) {
        fmt::println("  isAtLeast({}, {}, {})  ->  {:<5}   (Bibliothek ist {}.{}.{})", M, m, p,
                     version::isAtLeast(M, m, p), version::major(), version::minor(), version::patch());
    }

    /* ------------------------------------------------------------------ */
    printHeading("Bonus 3: Dieselben Daten über die C-API");
    fmt::println("  version_full_c()             = {}", version_full_c());
    fmt::println("  version_is_at_least_c(0,1,0) = {}", version_is_at_least_c(0, 1, 0));

    /* ------------------------------------------------------------------ */
    printHeading(fmt::format("Bonus 4: Wo liegt der Code? (dieses Programm ist {} gelinkt)", TEA_LINKAGE));
    printOrigin("main()", addressOf(&main));
    printOrigin("tea::version::fullVersion()", addressOf(&version::fullVersion));
    printOrigin("version_full_c()", addressOf(&version_full_c));
    printOrigin("std::printf() (libc)", addressOf(&std::printf));

    return 0; /* exit gracefully*/
}
