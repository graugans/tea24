# Übungsaufgabe: Statische Bibliothek für Projekt-Versionierung

## Notwendige git Kommandos

```sh
git status
git branch -a
git switch main
git fetch --all
# create a new local branch based on exercise-003
git switch -c solution-003 upstream/main
# perform changes
git add exercise-003 libs/version CMakeLists.txt
git commit -m "feat: solve exercise-003 static version library"
# push the changes to the cloud
git push -u origin solution-003
```

## Ziel der Aufgabe

In dieser Aufgabe erstellen Sie **alles selbst**:

- die Übungsdateien in `exercise-003/`
- eine **statische Bibliothek** in `libs/version`
- eine versionsbezogene Konfiguration über `*.h.in` + `configure_file(...)` (analog zu `config.h.in`)

Im Ordner `exercise-003/` liegt anfangs nur diese `README.md`.

---

## Lernziele

- Aufbau und Verwendung einer **statischen Bibliothek** (`add_library(... STATIC ...)`)
- Trennung von Interface (Header) und Implementierung (`.cpp`)
- Verständnis von `PUBLIC` / `PRIVATE` Include-Pfaden und Link-Abhängigkeiten
- Erzeugen von Headern zur Build-Zeit mit `configure_file(...)`
- Saubere CMake-Struktur mit Unterverzeichnissen

---

## Teil 1: Projektstruktur anlegen

Legen Sie folgende Struktur an:

```text
libs/
└── version/
    ├── CMakeLists.txt
    ├── version_config.h.in
    ├── include/
    │   └── version/
    │       └── version.h
    └── src/
        └── version.cpp

exercise-003/
├── CMakeLists.txt
├── main.cpp
├── config.h.in
└── README.md
```

Zusätzlich muss im Root-[CMakeLists.txt](/workspaces/tea24/CMakeLists.txt) sowohl die Bibliothek als auch die Übung eingebunden werden:

```cmake
add_subdirectory(libs/version)
add_subdirectory(exercise-003)
```

---

## Teil 2: API der Bibliothek definieren

Definieren Sie in `version.h` eine kleine API, z. B.:

- `const char* projectName();`
- `int major();`
- `int minor();`
- `int patch();`
- `std::string fullVersion();` (z. B. `tea24-0.0.1`)

Implementieren Sie die Funktionen in `version.cpp`.

Wichtig: Verwenden Sie in eigenen Headern klassische Include-Guards (`#ifndef / #define / #endif`).

---

## Teil 3: Version per `*.h.in` erzeugen

Erstellen Sie `version_config.h.in` in der Bibliothek und lassen Sie daraus beim Build einen Header generieren:

Beispielinhalte:

```cpp
#ifndef VERSION_CONFIG_H
#define VERSION_CONFIG_H

#define VERSION_LIB_NAME "@PROJECT_NAME@"
#define VERSION_LIB_VERSION_MAJOR @PROJECT_VERSION_MAJOR@
#define VERSION_LIB_VERSION_MINOR @PROJECT_VERSION_MINOR@
#define VERSION_LIB_VERSION_PATCH @PROJECT_VERSION_PATCH@

#endif
```

In `libs/version/CMakeLists.txt`:

1. `configure_file(...)` verwenden
2. die generierte Header-Location in Include-Pfade aufnehmen
3. die statische Bibliothek `version` erzeugen

---

## Teil 4: Statische Bibliothek in `exercise-003` verwenden

In `exercise-003/CMakeLists.txt`:

1. Executable `exercise-003` anlegen
2. Gegen `version` linken

Da `libs/version` bereits im Root-CMake eingebunden wurde, ist hier **kein weiteres `add_subdirectory(...)`** nötig.

In `main.cpp`:

- Versionsinfos über die Bibliothek abrufen
- mit `{fmt}` ausgeben

Beispiel:

```txt
Project: tea24
Version: 0.0.1
Full: tea24-0.0.1
```

---

## Teil 5: Verifizieren

```sh
cmake --build build
./build/bin/exercise-003
```

Zusätzlich prüfen:

- Liegt die statische Bibliothek unter `build/lib`? (z. B. `libversion.a`)
- Wird das Programm korrekt gegen diese Bibliothek gelinkt?

---

## Abgabe / kurzes Fazit

Beschreiben Sie kurz:

1. Unterschied zwischen Bibliotheks-Header und generiertem Konfig-Header
2. Warum `configure_file(...)` für Versionsdaten nützlich ist
3. Wie `PUBLIC` Include-Pfade und Link-Abhängigkeiten zusammenspielen

---

## Warum Bibliotheken? (Kontext)

Die Auslagerung in `libs/version` hat klare Vorteile:

- **Wiederverwendung:** dieselbe Versionierungslogik kann in mehreren Programmen verwendet werden
- **Zentrale Wartung:** Änderungen passieren an einer Stelle statt in vielen `main.cpp`-Dateien
- **Klarere Architektur:** API (`include/`) getrennt von Implementierung (`src/`)
- **Bessere Testbarkeit:** Bibliotheken lassen sich isoliert testen

Für diese Aufgabe ist das Ziel also nicht nur „es läuft“, sondern auch „es ist sauber wiederverwendbar“.

---

## Extended (Bonus): Zusatzaufgaben

### Bonus 1: API stabil halten

Erweitern Sie die Bibliothek um:

- `bool isAtLeast(int major, int minor, int patch);`

und testen Sie mehrere Vergleichsfälle.

### Bonus 2: Wiederverwendung demonstrieren (zweiter Nutzer der Bibliothek)

Legen Sie in `exercise-003` ein zweites kleines Programm an (z. B. `version_cli.cpp`) und linken Sie es ebenfalls gegen `version`.

Ziel:

- dieselbe Bibliothek wird von **mehr als einem** Executable verwendet
- kein kopierter Versionierungs-Code
- beide Programme liefern konsistente Versionsausgaben

Typische CMake-Erweiterung:

```cmake
add_executable(version_cli version_cli.cpp)
target_link_libraries(version_cli PRIVATE version fmt::fmt)
```

### Bonus 3: C-API zusätzlich anbieten + Name Mangling verstehen

Stellen Sie neben der C++-API auch eine C-kompatible API bereit (`extern "C"`), z. B.:

- `const char* version_full_c();`

Konkretes Beispiel (Header):

```cpp
/* version_c_api.h */
#ifndef VERSION_C_API_H
#define VERSION_C_API_H

#ifdef __cplusplus
extern "C" {
#endif

const char* version_full_c(void);

#ifdef __cplusplus
}
#endif

#endif
```

Konkretes Beispiel (Implementierung in C++):

```cpp
/* version_c_api.cpp */
#include "version_c_api.h"
#include "version/version.h"

const char* version_full_c(void)
{
    static std::string buffer;
    buffer = fullVersion();   // kommt aus der C++-API
    return buffer.c_str();    // C-kompatibel: const char*
}
```

Optionaler Test aus C:

```c
/* c_consumer.c */
#include <stdio.h>
#include "version_c_api.h"

int main(void) {
    printf("Version: %s\n", version_full_c());
    return 0;
}
```

Warum?

- C++ verändert Funktionsnamen intern (Name Mangling), damit z. B. Overloads aufgelöst werden können.
- C erwartet unveränderte Symbolnamen.
- `extern "C"` deaktiviert für diese Funktionen das C++-Mangling.

Symbolvergleich mit Tooling (Unixoid):

```sh
# C++-Symbole (oft "vermangelt")
nm -C build/lib/libversion.a

# Ohne -C sieht man die rohen Symbole
nm build/lib/libversion.a

# Alternative auf Objektdateien/ELF:
objdump -t build/lib/libversion.a
readelf -Ws build/bin/exercise-003
```

Schauen Sie gezielt:

- Wie heißt die C++-Funktion als Symbol?
- Wie heißt die gleiche Funktion mit `extern "C"`?
- Welche Symbole sind leichter aus C heraus nutzbar?

### Bonus 4: Statische vs. dynamische Bibliothek

Erzeugen Sie optional zusätzlich eine **shared library** und vergleichen Sie:

- Build-Ergebnis
- Artefakte (`.a` vs. `.so` auf Linux, `.dll` + Import-`.lib` auf Windows)
- Link-Verhalten

Leitfragen:

- Wann wird Code ins Zielprogramm kopiert (statisch)?
- Wann wird erst zur Laufzeit geladen (dynamisch)?
- Welche Konsequenzen hat das für Speicherbedarf, Deployment und Updates?

### Bonus 5: Typische Anfängerfehler absichtlich erzeugen

1. Header vergessen zu includieren
2. `target_link_libraries(...)` vergessen
3. Include-Pfade nur `PRIVATE`, obwohl sie `PUBLIC` sein müssten
4. `configure_file(...)` erzeugt Header, aber Include-Pfad zeigt nicht auf den Build-Ordner

Dokumentieren Sie jeweils Fehlermeldung, Ursache und Fix.

### Bonus 6: „Klausurmodus“ – mögliche Theoriefragen

1. Was ist der Unterschied zwischen statischer und dynamischer Bibliothek?
2. Typische Dateiendungen/Namen:
   - Linux/Unix: statisch `lib<name>.a`, dynamisch `lib<name>.so`
   - Windows: statisch `<name>.lib`, dynamisch `<name>.dll` (oft mit Import-`<name>.lib`)
3. Welche Bibliotheksform wird im Mikrocontroller-Umfeld häufig genutzt und warum?
   - Typisch: **statische Bibliotheken**, weil Deployment und Laufzeitumgebung kontrollierter sind und oft kein dynamischer Loader vorhanden ist.
4. Was bedeutet Name Mangling, und wozu braucht man `extern "C"`?

---

## Hast du etwas vergessen? (Checkliste)

- [ ] `exercise-003` in Root-[CMakeLists.txt](/workspaces/tea24/CMakeLists.txt) eingebunden
- [ ] `libs/version` in Root-[CMakeLists.txt](/workspaces/tea24/CMakeLists.txt) eingebunden
- [ ] Bibliothek wirklich als `STATIC` angelegt
- [ ] Generierter Header wird gefunden (Build-Include-Pfad korrekt)
- [ ] Bibliothek exportiert ein klares Interface (`include/version/version.h`)
- [ ] Executable linkt gegen `version`
- [ ] Ausgabe zur Laufzeit zeigt korrekte Versionsdaten
