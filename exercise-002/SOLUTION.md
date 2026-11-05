## Aufgabe 1

Die Ausgabe lautet:

```
[doSomethingStatic] doSomethingStatic, value: 56, address of value: 0x5bd2b91b6028
[doSomethingStatic] doSomethingStatic, noneStatic: 42, address of noneStatic: 0x7fff4218de80
[doSomethingStatic] doSomethingStatic, value: 57, address of value: 0x5bd2b91b6028
[doSomethingStatic] doSomethingStatic, noneStatic: 42, address of noneStatic: 0x7fff4218de10
```
Durch den einmaligen rekursiven Aufruf sehen wir das die Stacklokale Variable `int noneStatic = 42;` mit jedem Durchlauf ihren initialen Wert behält. Die Adresse der Variablen wird im verschachtelten Aufruf auch kleiner (0x7fff4218de10), da der Stack nach unten wächst. Die Addresse der statischen Variable Value bleibt gleich und der Wert ändert sich durch das inkrement bei jedem Aufruf.

## Aufgabe 2

Das wurde geändert:
```diff
diff --git a/exercise-002/main.cpp b/exercise-002/main.cpp
index aa4cded..8a75243 100644
--- a/exercise-002/main.cpp
+++ b/exercise-002/main.cpp
@@ -48,5 +48,9 @@ auto main(int argc, char** argv) -> int
     fmt::print("array_ptr[0] {}\n", array_ptr[0]);
     fmt::print("big_data_blob[0] {}\n", big_data_blob[0]);
 */
+
+    printModuleStaticValues();
+    printModuleStaticValues();
+
     return 0; /* exit gracefully*/
 }
diff --git a/exercise-002/module.cpp b/exercise-002/module.cpp
index 1e41cff..0f0fdb5 100644
--- a/exercise-002/module.cpp
+++ b/exercise-002/module.cpp
@@ -17,6 +17,10 @@ const int rodata2 = 43;
 
 const int big_data_blob[] = {42,0xff};
 
+
+static int moduleCounter;
+static int moduleValue = 123;
+
 void doSomething(int value)
 {
     value = value + 1;
@@ -69,3 +73,27 @@ void doSomethingStatic()
         doSomethingStatic();
     }
 }
+
+void printModuleStaticValues() {
+    moduleCounter = moduleCounter + 1;
+    fmt::println(
+        "[{}], moduleCounter: {}, address of moduleCounter: {}",
+         __func__, 
+         moduleCounter, 
+         fmt::ptr(&moduleCounter)
+        );
+
+    fmt::println(
+        "[{}], moduleValue: {}, address of moduleValue: {}",
+         __func__, 
+         moduleValue, 
+         fmt::ptr(&moduleValue)
+        );
+    fmt::println(
+        "[{}], data(.data): {}, address of data: {}",
+         __func__, 
+         data, 
+         fmt::ptr(&data)
+        );
+    moduleValue = moduleValue + 42;
+}
\ No newline at end of file
diff --git a/exercise-002/module.h b/exercise-002/module.h
index a2f58cf..3949dd8 100644
--- a/exercise-002/module.h
+++ b/exercise-002/module.h
@@ -11,6 +11,19 @@ extern const int rodata;
 extern const int rodata2;
 extern const int big_data_blob[2];
 
+/*
+This is not possible due the fact moduleCounter is static
+
+[build] /workspaces/tea24/exercise-002/module.cpp:21:12: error: static declaration of 'moduleCounter' follows non-static declaration
+[build]    21 | static int moduleCounter;
+[build]       |            ^
+[build] /workspaces/tea24/exercise-002/module.h:17:12: note: previous declaration is here
+[build]    17 | extern int moduleCounter;
+[build]       |            ^
+
+extern int moduleCounter;
+*/
+
 void doSomething(int value);
 
 void doSomethingPtr(int* ptr);
@@ -19,6 +32,6 @@ int* doReturnAPtr();
 
 void doSomethingStatic();
 
-
+void printModuleStaticValues();
 
 #endif
```

Durch das Schlüsselwort `static` auf Modulebene bleibt die Sichtbarkeit auf das Modul beschränkt. Das lässt sich mit einem modernen Compiler auch nicht mehr ändern.

Frage: was passiert bei einem Pointer auf eine Modulstatische Variable?

## Aufgabe 3

```sh
nm -SC ./build/bin/exercise-002
```

Das `-C` wird benötigt um das NameMangling von C++ aufzulösen.

Vorher:
```
0000000000004028 0000000000000004 d _ZZ17doSomethingStaticvE5value
```

Nachher:
```
0000000000004028 0000000000000004 d doSomethingStatic()::value
```

Häufige Typbuchstaben in deiner nm Ausgabe

    B / b: BSS, uninitialisierte oder nullinitialisierte Daten. Großbuchstabe bedeutet üblicherweise global sichtbar, kleinbuchstabe lokal.
    D / d: initialisierte, beschreibbare Daten, typischerweise .data.
    R / r: schreibgeschützte Daten, typischerweise .rodata.
    T / t: Programmcode, typischerweise .text.
    U: undefiniertes Symbol — wird von einer anderen Objektdatei oder Bibliothek bereitgestellt.
    W / w: schwaches Symbol.

HINWEIS: Die Buchstaben sind eine hilfreiche Zuordnung, aber nicht für jede ELF-Sektion eine vollständige oder eindeutige Beschreibung.

## Aufgabe 4 (Zusatz)

```
[main] Value of shadow 11 address of shadow 0x5d3b19163020
[printTheModuleShadow], shadow: 42, address of shadow: 0x5d3b19163034
```