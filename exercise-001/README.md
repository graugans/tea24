# Übungsaufgabe: Zeiger in C++ mit `fmt`

## Notwendige git Kommandos

```sh
git status
git branch -a
git switch main
git fetch --all
# create a new local branch based on the origin main
git switch -c solution-001 upstream/main
# perform changes
git add exercise-001
git commit -m "feat: add exercise number two"
# push the changes to the cloud
git push -u origin solution-001
....
```

## Programm kompilieren und ausführen

Das Projekt verwendet CMake als Build-System. So kompilieren und starten Sie das Programm:

```sh
# Projekt kompilieren (NUR aus dem Hauptverzeichnis) alternativ über die GUI 
cmake --build build

# Programm ausführen
./build/bin/exercise-001
```

### Warum `./` vor dem Programmnamen?

In Linux/Unix-Systemen ist das aktuelle Verzeichnis (`.`) aus Sicherheitsgründen **nicht** automatisch im `PATH`. Deshalb müssen Sie explizit angeben, dass sich das Programm im aktuellen Pfad befindet:

- **`./`** bedeutet "aktuelles Verzeichnis"
- **`build/bin/exercise-001`** ist der relative Pfad zur ausführbaren Datei
- **Ohne `./`** würde Linux nur in den `PATH`-Verzeichnissen suchen (z.B. `/usr/bin`, `/bin`)

```sh
# ✅ Richtig - expliziter Pfad
./build/bin/exercise-001

# ❌ Falsch - Programm wird nicht gefunden
build/bin/exercise-001  # könnte funktionieren, aber ./ist expliziter

# ❌ Falsch - nur Programmname ohne Pfad
exercise-001  # "command not found"
```

> **Tipp:** Sie können auch den absoluten Pfad verwenden: `/workspaces/tea23/build/bin/exercise-001`

## Die `main`-Funktion: `argc` und `argv`

In Desktop-/Server-Programmen hat die `main`-Funktion oft Parameter für Kommandozeilenargumente:

```cpp
int main(int argc, char **argv)
```

### Was bedeuten `argc` und `argv`?

- **`argc`** (argument count): Anzahl der Kommandozeilenargumente (inklusive Programmname)
- **`argv`** (argument vector): Array von Zeigern auf die Argument-Strings

### Praktisches Beispiel

```sh
# Programm mit Argumenten aufrufen
./build/bin/exercise-001 --verbose debug.txt 42
```

In diesem Fall wäre:

```cpp
argc = 4                    // Anzahl der Argumente
argv[0] = "./build/bin/exercise-001"  // Programmname (immer das erste Argument)
argv[1] = "--verbose"       // Erstes echtes Argument
argv[2] = "debug.txt"       // Zweites Argument  
argv[3] = "42"              // Drittes Argument
argv[4] = nullptr           // Array-Ende-Markierung
```

### Mikrocontroller vs. Desktop

**Desktop/Server (`int main(int argc, char **argv)`):**

- Kommandozeilenargumente verfügbar
- Betriebssystem ruft `main` auf
- Program kann beendet werden (`return 0;`)

**Mikrocontroller (`int main(void)` oder Endlosschleife):**

```cpp
int main(void) {
    // Initialisierung
    while(1) {
        // Hauptprogramm läuft endlos
    }
    // return wird nie erreicht
}
```

- Keine Kommandozeile vorhanden
- Programm läuft endlos (kein Betriebssystem zum Zurückkehren)
- Oft auch `setup()` und `loop()` Funktionen (Arduino-Stil)

**Lernziel:** Wiederholung und Festigung des Verständnisses von Zeigern in C/C++ sowie Einführung in die Verwendung der [`fmt`](https://fmt.dev/)-Bibliothek zur Ausgabe.

---

## Auffrischung: Include-Dateien und Header

Bevor wir mit den Zeigern beginnen, eine kurze Auffrischung zu einem wichtigen Grundkonzept:

### Was sind Include-Dateien?

Include-Dateien (Header) enthalten **Deklarationen** von Funktionen, Klassen und Konstanten, die wir in unserem Code verwenden möchten. Sie sagen dem Compiler, was verfügbar ist, aber nicht wie es implementiert ist.

```cpp
#include <iostream>     // Für std::cout, std::cin
#include <cstdlib>      // Für malloc, free
#include <fmt/format.h> // Für fmt::print, fmt::println
```

### Unterschied zwischen `<>` und `""`

#### Systemheader mit `<>`

```cpp
#include <iostream>     // Standard-Bibliothek
#include <vector>       // STL-Container
#include <fmt/format.h> // Externe Bibliothek (installiert)
```

- **Suchpfad**: Compiler sucht in System-/Bibliotheksverzeichnissen
- **Verwendung**: Standard-C++-Bibliothek, installierte externe Bibliotheken

#### Eigene Header mit `""`

```cpp
#include "config.h"     // Eigene Header-Datei
#include "myclass.h"    // Eigene Klassen
#include "../utils.h"   // Relative Pfade möglich
```

- **Suchpfad**: Compiler sucht zuerst im aktuellen Verzeichnis, dann in System-Verzeichnissen
- **Verwendung**: Selbst geschriebene Header-Dateien, projektspezifische Dateien

> **Faustregel**: `<>` für Standard- und externe Bibliotheken, `""` für eigene Dateien!

---

## Die `fmt`-Bibliothek: Eine moderne Alternative zu `printf`

Wenn Sie aus der C-Programmierung kommen, kennen Sie vermutlich `printf()` für die Ausgabe:

```c
printf("Wert: %d, Adresse: %p\n", x, &x);
```

Die `fmt`-Bibliothek ist eine moderne C++-Alternative, die **einfacher und sicherer** ist:

> **Python-Analogie:** Wenn Sie Python kennen, ist `fmt::print()` sehr ähnlich zu Pythons `print()` mit f-strings!

### Vorteile von `fmt::print()`

- **Typsicherheit**: Keine Formatierungsfehler durch falsche Platzhalter
- **Einfachere Syntax**: `{}` statt `%d`, `%p`, `%s` etc.
- **Automatische Typenerkennung**: Kein manuelles Angeben des Datentyps nötig
- **Bessere Lesbarkeit**: Modernere, intuitivere Syntax

### Vergleich C vs. fmt

```c
// C-Stil mit printf
printf("Name: %s, Alter: %d, Gehalt: %.2f\n", name, age, salary);
```

```cpp
// Modern mit fmt
fmt::print("Name: {}, Alter: {}, Gehalt: {:.2f}\n", name, age, salary);
```

```python
# Python f-string (sehr ähnlich!)
print(f"Name: {name}, Alter: {age}, Gehalt: {salary:.2f}")
```

### Praktisches Beispiel für diese Übung

```c
// C-Stil
int x = 42;
printf("Wert: %d, Adresse: %p\n", x, &x);
```

```cpp
// Mit fmt (wie Python!)
int x = 42;
fmt::print("Wert: {}, Adresse: {}\n", x, fmt::ptr(&x));
```

```python
# Python (konzeptionell ähnlich)
x = 42
print(f"Wert: {x}, Adresse: {id(x)}")  # id() zeigt Speicheradresse
```

### Wichtige `fmt`-Funktionen für diese Übung

- `fmt::print("Text")` - Einfache Textausgabe
- `fmt::print("Wert: {}\n", variable)` - Variable ausgeben  
- `fmt::println("Text")` - Textausgabe **mit automatischem Zeilenendezeichen**
- `fmt::println("Wert: {}", variable)` - Variable ausgeben mit automatischem `\n`
- `fmt::ptr(zeiger)` - Zeiger/Adresse formatiert ausgeben

> **Tipp:** `fmt::print` fügt **kein automatisches Zeilenendezeichen** hinzu - verwenden Sie `\n` am Ende!  
> **Noch besser:** Nutzen Sie `fmt::println` - dann brauchen Sie kein manuelles `\n`!

---

## Übungsaufgabe: Speichersegmente in C++

**Lernziel:** Verständnis der verschiedenen Speichersegmente (Stack, Heap, Data, Code) in einem C++-Programm.

---

### Teil 1: Globale Variablen

1. Deklarieren Sie eine **globale Variable** `int globalVar = 1;`.
2. Geben Sie im `main` den Wert und die Adresse von `globalVar` mit `fmt::print` aus.
3. Beobachten Sie die Adresse der Variable.
4. Deklarieren Sie mehre **globale Variable** und weisen Sie diese Werte zu. Beobachten Sie dabei die Adressen und notieren sie sich Ihre Beobachtungen.
    - beliebige
    - Nullwert
    - keine Werte
5. Was passiert wenn die Schlüsselwörter `static` und `const` verwendet werden?

---

### Teil 2: Lokale Variablen (Stack)

1. Deklarieren Sie im `main` eine lokale Variable `int localVar = 2;`.
2. Geben Sie den Wert und die Adresse von `localVar` mit `fmt::print` aus.
3. Was passiert wenn die Schlüsselwörter `static` und `const` verwendet werden?
4. Wie ändert sich die Adresse im Vergleich zu 1.4 und 1.5

---

### Teil 3: Dynamisch allozierte Variablen (Heap)

1. Reservieren Sie mit `new` Speicher für eine einzelne `int`-Variable (`int* heapVar = new int(3);`).
2. Geben Sie den Wert und die Adresse von `heapVar` mit `fmt::print` aus.
3. Wie ändert sich die Adresse im Vergleich zu 1.4 und 1.5
4. Geben Sie den Speicher mit `delete` wieder frei.

---

### Teil 4: Funktionen (Code-Segment)

1. Definieren Sie eine Funktion `void foo() { fmt::print("Hello from foo!\\n"); }`.
2. Geben Sie im `main` mit `fmt::print("Adresse von foo: {}\\n", fmt::ptr(&foo));` die Adresse der Funktion aus.
3. Beobachten Sie: Funktionen liegen im **Code-Segment**.

---

### Zusatz

Schreiben Sie ein kurzes Fazit, in dem Sie erklären:

- Welche Art von Variablen wo gespeichert wird.
- Warum es wichtig ist, zwischen Stack und Heap zu unterscheiden.
- Welche Rolle Zeiger in diesem Zusammenhang spielen.
- Erstellen Sie eine Skizze des gesamten Arbeitsspeicher.
