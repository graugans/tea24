# Übungsaufgabe: `static` in Funktion und auf Modulebene

## Notwendige git Kommandos

```sh
git status
git branch -a
git switch main
git fetch --all
# create a new local branch based on exercise-002
git switch -c solution-002 upstream/exercise-002
# perform changes
git add exercise-002
git commit -m "feat: solve exercise-002 static task"
# push the changes to the cloud
git push -u origin solution-002
```

## Programm kompilieren und ausführen

Das Projekt verwendet CMake als Build-System:

```sh
# Projekt kompilieren (NUR aus dem Hauptverzeichnis)
cmake --build build

# Programm ausführen
./build/bin/exercise-002
```

## Kontext: Übersetzungseinheit und Modul

In dieser Aufgabe arbeiten Sie mit zwei Dateien:

- `module.h` enthält die Schnittstelle (Deklarationen)
- `module.cpp` enthält die Implementierung (Definitionen)

Damit üben Sie den Unterschied zwischen:

- **Sichtbarkeit** (wer darf ein Symbol sehen?)
- **Lebensdauer** (wie lange existiert eine Variable?)
- **Speichersegment** (wo liegt die Variable im Programm?)

### Was ist eine Übersetzungseinheit?

Eine **Übersetzungseinheit** (translation unit) ist vereinfacht gesagt:

- eine `.cpp`-Datei
- plus alle Inhalte, die über `#include` dort eingefügt werden

Für den Compiler sind `main.cpp` und `module.cpp` daher zwei getrennte Übersetzungseinheiten.  
Jede Übersetzungseinheit wird zuerst einzeln kompiliert; erst danach werden die Objektdateien zusammen gelinkt.

### Warum braucht man Header-Dateien?

Header-Dateien (`.h`) enthalten typischerweise **Deklarationen**, z. B.:

- Funktionsdeklarationen
- `extern`-Deklarationen von globalen Variablen
- Typdefinitionen (`struct`, `class`, `enum`)

Der Nutzen:

- Andere Übersetzungseinheiten kennen die Schnittstelle, ohne die Implementierung zu duplizieren.
- Eine Funktion/Variable wird genau einmal **definiert** (z. B. in `module.cpp`), kann aber an vielen Stellen verwendet werden.

### Warum braucht man `extern`?

`extern` sagt:  
**„Diese globale Variable ist hier nur deklariert; die eigentliche Definition liegt in einer anderen Übersetzungseinheit.“**

Beispielprinzip:

- In `module.h`: `extern int data;`  → nur Deklaration
- In `module.cpp`: `int data = 45;`  → Definition (Speicher wird angelegt)

Ohne `extern` in einem Header würden bei Mehrfach-Include schnell mehrere Definitionen entstehen (Linkerfehler).

### Was ist ein Include Guard?

Ein Include Guard verhindert, dass derselbe Header in einer Übersetzungseinheit mehrfach verarbeitet wird.

Typisches Muster:

```cpp
#ifndef MY_MODULE_H
#define MY_MODULE_H

// Header-Inhalt

#endif
```

Warum wichtig?

- Schützt vor doppelten Deklarationen/Definitionen innerhalb einer Übersetzungseinheit
- Verhindert schwer verständliche Compilerfehler bei verschachtelten Includes

## Lernziele

- Vertiefung der git Kenntnisse
- Wiederholung des Modul-Konzepts in C/C++ (`.h` + `.cpp`)
- Sicherer Umgang mit der `{fmt}` Bibliothek
- Verständnis des Schlüsselworts `static`
  - zuerst auf **Funktionsebene**
  - danach auf **Modulebene**
- Einordnung von Variablen in `.bss`, `.data` und `.rodata`

---

## Teil 1: `static` innerhalb einer Funktion

Nutzen Sie die vorhandene Funktion `doSomethingStatic()` in `module.cpp`.

1. Führen Sie das Programm aus und beobachten Sie die Ausgabe von `doSomethingStatic()`.
2. Erweitern Sie die Funktion so, dass zusätzlich die Adresse der statischen Variablen ausgegeben wird.
3. Rufen Sie `doSomethingStatic()` mehrfach auf und beantworten Sie:
   - Bleibt die Adresse konstant?
   - Warum wird der Wert bei jedem Aufruf weiter erhöht?
4. Vergleichen Sie das Verhalten mit einer **nicht-statischen** lokalen Variable in derselben Funktion.

**Ziel:** Verstehen, dass eine `static`-lokale Variable nur einmal initialisiert wird und zwischen Funktionsaufrufen ihren Zustand behält.

---

## Teil 2: `static` auf Modulebene (`module.cpp`)

Jetzt betrachten Sie `static` außerhalb einer Funktion.

1. Legen Sie in `module.cpp` mindestens zwei module-lokale Variablen an, z. B.:
   - `static int moduleCounter;`
   - `static int moduleValue = 123;`
2. Erstellen Sie passende Funktionen in `module.cpp`, um Werte und Adressen dieser Variablen auszugeben.
3. Deklarieren Sie diese Variablen **nicht** in `module.h`.
4. Prüfen Sie, dass die Variablen nur im Modul sichtbar sind (internal linkage).

**Ziel:** Verstehen, dass `static` auf Dateiebene die Sichtbarkeit auf die aktuelle Übersetzungseinheit begrenzt.

---

## Teil 3: In welchem Segment landen die Variablen?

Untersuchen Sie, in welchem Segment die verschiedenen Variablen landen:

- `static int moduleCounter;` (ohne Initialisierung)
- `static int moduleValue = 123;` (mit Initialisierung)
- `static const int moduleConst = 77;`
- `static`-lokale Variable aus `doSomethingStatic()`

Nutzen Sie die Programmausgabe plus Tooling:

```sh
# Segmentgrößen
size --format=sysv ./build/bin/exercise-002

# Symbole inkl. Segment-Typen (optional)
nm -S ./build/bin/exercise-002
```

Leiten Sie daraus ab:

- Welche Variablen typischerweise in `.bss` liegen
- Welche in `.data` liegen
- Wann `const`-Daten in `.rodata` landen

---

## Abgabe / kurzes Fazit

Dokumentieren Sie in wenigen Sätzen:

1. Unterschied zwischen `static` in einer Funktion und `static` auf Modulebene
2. Welche Ihrer Beispielvariablen in `.bss`, `.data` oder `.rodata` liegen
3. Warum `static` für Kapselung in Modulen nützlich ist

---

## Extended (Bonus): Lernen durch gezielte Fehler

Dieser Abschnitt ist für alle, die schneller vorankommen und tiefer einsteigen möchten.

### Bonus 1: Gleichnamige „globale“ Variablen in `main.cpp` und `module.cpp`

1. Legen Sie in `module.cpp` eine Datei-Variable an (z. B. `static int shadow = 11;`).
2. Legen Sie in `main.cpp` ebenfalls eine globale Variable mit **dem gleichen Namen** an (z. B. `int shadow = 99;`).
3. Geben Sie in beiden Übersetzungseinheiten Wert und Adresse mit `fmt::print` aus.
4. Beobachten Sie:
   - Welche Funktion greift auf welche Variable zu?
   - Warum sind die Adressen unterschiedlich?
   - Welche Rolle spielt `static` auf Modulebene dabei?

**Hinweis:** Diese Aufgabe zeigt sehr gut den Unterschied zwischen Dateisichtbarkeit (internal linkage) und Programm-Sichtbarkeit (external linkage).

### Bonus 2: Include Guard absichtlich weglassen

1. Erstellen Sie eine neue Header-Datei, z. B. `bonus_config.h`.
2. Schreiben Sie zuerst **ohne Include Guard** eine kleine Deklaration hinein (z. B. Konstante oder Funktionsprototyp).
3. Binden Sie diesen Header in `module.h` ein.
4. Binden Sie denselben Header zusätzlich direkt in `module.cpp` ein.
5. Kompilieren Sie und analysieren Sie die Fehlermeldung.
6. Ergänzen Sie danach einen Include Guard (`#ifndef / #define / #endif`) und prüfen Sie, dass der Build wieder funktioniert.

**Hinweis zu CMake und Include-Pfaden:**

- Wenn `bonus_config.h` im selben Verzeichnis wie `module.h`/`module.cpp` liegt (also in `exercise-002/`), ist meist **keine CMake-Änderung nötig**.
- Grund: `#include "bonus_config.h"` sucht zuerst relativ zur einbindenden Datei; zusätzlich kennt der Compiler bei diesem Projekt ohnehin das aktuelle Quellverzeichnis.
- Eine CMake-Anpassung wird erst relevant, wenn Header in Unterordner ausgelagert werden (z. B. `exercise-002/include/`).

Typische CMake-Erweiterung dafür:

```cmake
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

**Was macht der Präprozessor dabei?**

- Er ersetzt `#include` vor dem eigentlichen Kompilieren durch den Dateiinhalt.
- Bei `#include "datei.h"` wird zuerst lokal (Datei-/Projektpfad), dann in weiteren Include-Pfaden gesucht.
- Bei `#include <datei.h>` wird primär in den konfigurierten/systemweiten Include-Pfaden gesucht.

### Bonus 3: Typische Anfängerfehler rund um Header & Linker

Untersuchen Sie jeweils Fehlermeldung, Ursache und Korrektur:

1. **Definition im Header statt Deklaration**  
   Beispiel: `int x = 5;` in einer `.h`-Datei, die mehrfach inkludiert wird.
2. **`extern` in der `.cpp`-Datei vergessen zu definieren**  
   Deklaration existiert, aber es gibt keine echte Definition.
3. **Funktionssignatur passt nicht zur Deklaration**  
   In `.h` und `.cpp` unterschiedliche Parameter oder `const`-Qualifizierer.
4. **Fehlendes `const` im Interface**  
   Zugriff über Zeiger/Referenz funktioniert nicht wie erwartet, weil `const`-Korrektheit verletzt wird.

### Bonus 4: Segmentanalyse mit Null- und Nicht-Null-Initialisierung

Vergleichen Sie diese Varianten und prüfen Sie den Effekt mit `nm -S` und `size`:

- `int a;`
- `int b = 0;`
- `int c = 1;`
- `const int d = 1;`

Beantworten Sie:

- Warum landen `a` und oft auch `b` typischerweise in `.bss`?
- Warum liegt `c` in `.data`?
- Unter welchen Bedingungen erscheint `d` in `.rodata`?
