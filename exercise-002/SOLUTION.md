## Aufgabe 1

Die Ausgabe lautet:

```
[doSomethingStatic] doSomethingStatic, value: 56, address of value: 0x5bd2b91b6028
[doSomethingStatic] doSomethingStatic, noneStatic: 42, address of noneStatic: 0x7fff4218de80
[doSomethingStatic] doSomethingStatic, value: 57, address of value: 0x5bd2b91b6028
[doSomethingStatic] doSomethingStatic, noneStatic: 42, address of noneStatic: 0x7fff4218de10
```
Durch den einmaligen rekursiven Aufruf sehen wir das die Stacklokale Variable `int noneStatic = 42;` mit jedem Durchlauf ihren initialen Wert behält. Die Adresse der Variablen wird im verschachtelten Aufruf auch kleiner (0x7fff4218de10), da der Stack nach unten wächst. Die Addresse der statischen Variable Value bleibt gleich und der Wert ändert sich durch das inkrement bei jedem Aufruf.


