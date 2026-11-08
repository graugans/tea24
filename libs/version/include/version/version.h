#ifndef TEA24_VERSION_VERSION_H
#define TEA24_VERSION_VERSION_H

/*
 * Öffentliche C++-API der Versionsbibliothek (liegt unter include/ -> PUBLIC).
 *
 * Bewusst KEIN Include von version_config.h: Der generierte Konfig-Header ist ein
 * Implementierungsdetail der Bibliothek (PRIVATE). Nutzer sehen nur Funktionen –
 * ändert sich die Version, muss nur die Bibliothek neu gebaut werden, nicht jeder Nutzer.
 *
 * Stolperfalle: glibc definiert in <sys/sysmacros.h> die Makros major(dev) und minor(dev).
 * Ist dieser Header vor version.h eingebunden, macht der Präprozessor aus
 * tea::version::major() still tea::version::gnu_dev_major() -> Linkerfehler.
 * Namespaces schützen NICHT vor Makros (siehe SOLUTION.md, Stolperfalle 1).
 */

#include <string>

namespace tea::version
{

/** Name der Bibliothek (CMake: PROJECT_NAME in libs/version) */
const char* projectName();

/** Einzelne Versionsbestandteile (CMake: PROJECT_VERSION_MAJOR/MINOR/PATCH) */
int major();
int minor();
int patch();

/** Komplette Versionskennung, z. B. "version_library-0.1.2" */
std::string fullVersion();

/** Datum, an dem CMake den Konfig-Header erzeugt hat (Configure-Zeit, nicht Build-Zeit!) */
const char* configureDate();

/**
 * Bonus 1: Ist die Bibliotheksversion mindestens major.minor.patch?
 * Vergleich lexikografisch: erst major, bei Gleichstand minor, dann patch.
 */
bool isAtLeast(int major, int minor, int patch);

}  // namespace tea::version

#endif /* TEA24_VERSION_VERSION_H */
