# Vom Quelltext zur ausführbaren Datei

## Überblick: Jede Stufe erzeugt eine andere Art von Datei

![Build-Prozess: von C/C++-Quellcode zu Maschinencode](img/graphviz.svg)

*Abbildung: Der Build-Prozess. Die Nummern (1) bis (4) stehen für die Stufen in der Tabelle und in Teil A.*

| Stufe | Befehl | Eingabe | Ausgabe | Typischer Fehler |
|---|---|---|---|---|
| (1) Präprozessor | `gcc -E` | `.c` | `.i` | Header nicht gefunden |
| (2) Compiler | `gcc -S` | `.i` | `.s` | Syntaxfehler, unbekannter Name |
| (3) Assembler | `gcc -c` | `.s` | `.o` | selten |
| (4) Linker | `gcc` | `.o` | ausführbare Datei | undefined reference, multiple definition |

Merksatz: Der Compiler kennt immer nur **eine** Datei. Der Linker füllt die Lücken zwischen den Dateien.

## Teil A: Eine Quelldatei, eine Header-Datei

Lege im Ordner  `exercise-004` zwei Dateien an.

**greet.h**

```c
#define ZEICHEN 'A'
```

**main.c**

```c
#include <stdio.h>
#include "greet.h"

int main(void)
{
    putc(ZEICHEN, stdout);
    putc('\n', stdout);
    return 0;
}
```

Führe die Schritte einzeln aus und trage ein, was du siehst. Die Schritte 1 bis 4 entsprechen den Stufen (1) bis (4) in der Abbildung.

| Schritt | Befehl | Was ich sehe |
|---|---|---|
| 1. Präprozessor | `gcc -E main.c -o main.i` |  |
| | `tail main.i` |  |
| 2. Compiler | `gcc -S main.i -o main.s` |  |
| | `cat main.s` |  |
| 3. Assembler | `gcc -c main.s -o main.o` |  |
| | `nm main.o` |  |
| | `objdump -dr main.o` |  |
| 4. Linker | `gcc main.o -o main` |  |
| | `./main` |  |
| | `objdump -d main` |  |

Fragen zu Teil A:

1. Was ist aus `ZEICHEN` in `main.i` geworden? ______________________________________
2. Enthält `stdio.h` den Code von `putc` oder nur eine Deklaration? ______________________
3. Was bedeutet das `U` vor `putc` in der Ausgabe von `nm main.o`? ______________________
4. Was steht bei `call putc` in `main.o`, was in `main`? ______________________________


NOTE: Commit und push nicht vergessen!

## Teil B: Zwei Quelldateien und der Linker

Lege einen Ordner `exercise-004-01` an mit diesen drei Dateien.

**greet.h**

```c
void gruss(void);
```

**greet.c**

```c
#include <stdio.h>
#include "greet.h"

void gruss(void)
{
putc('A', stdout);
putc('\n', stdout);
}
```

**main.c**

```c
#include "greet.h"

int main(void)
{
gruss();
return 0;
}
```

| Schritt | Befehl | Was ich sehe |
|---|---|---|
| 1. Einzeln übersetzen | `gcc -c main.c -o main.o` |  |
| | `gcc -c greet.c -o greet.o` |  |
| 2. Symbole | `nm main.o` |  |
| | `nm greet.o` |  |
| 3. Fehler provozieren | `gcc main.o -o main` |  |
| 4. Richtig linken | `gcc main.o greet.o -o main` |  |
| | `./main` |  |

Fragen zu Teil B:

1. Warum meldet der **Compiler** keinen Fehler, obwohl `gruss` in `main.c` nicht definiert ist? ______________________________________
2. Welche Stufe meldet *undefined reference*? ______________________
3. Was passiert, wenn du `gruss` zusätzlich in `main.c` definierst und beide Dateien linkst? Probiere es aus. ______________________

## Teil C: Dynamisch und statisch linken

Benutze die Dateien aus Teil B.

| Schritt | Befehl | Was ich sehe |
|---|---|---|
| 1. Dynamisch | `gcc main.o greet.o -o main` |  |
| | `ldd main` |  |
| | `file main` |  |
| 2. Statisch | `gcc -static main.o greet.o -o main_static` |  |
| | `ldd main_static` |  |
| | `file main_static` |  |
| 3. Größe | `ls -l main main_static` |  |
| 4. Symbol `putc` | `nm main \| grep putc` |  |
| | `nm main_static \| grep putc` |  |

Fragen zu Teil C:

1. Warum ist `main_static` so viel größer als `main`? ______________________
2. Wo liegt der Code von `putc` bei `main`, wo bei `main_static`? ______________________
3. Was passiert, wenn die libc auf dem Zielsystem fehlt? Welche der beiden Dateien läuft trotzdem? ______________________

## Hinweise

- Für `-static` braucht man die statische libc (`libc.a`). Unter Debian und Ubuntu ist sie in `libc6-dev` enthalten, unter Fedora kommt sie mit `glibc-static`.
- Neuere `gcc`-Versionen bauen standardmäßig PIE-Binaries. Wenn die `objdump`-Ausgabe verwirrt, ergänze `-no-pie` beim Übersetzen und Linken.
- Mit `gcc -v` siehst du, welche Einzelprogramme `gcc` im Hintergrund aufruft.