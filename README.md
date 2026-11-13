# Mikrocomputertechnik 3

Begleitprojekt für Mikrocomputertechnik 3 im Kurs tea24 an der DHBW Ravensburg Campus Friedrichshafen

**WICHTIG** Bitte verwenden sie zur Entwicklung die vorgefertigten Umgebungen.

## Musterlösungen

Die Musterlösungen liegen **nicht** auf dem `main` Branch, sondern im Branch
[`solutions`](https://github.com/graugans/tea24/tree/solutions). So bleiben die Aufgaben auf `main` unverändert, und
Sie können sie zuerst selbst lösen.

| Übungsaufgabe | Musterlösung |
| --- | --- |
| [Nr. 001](exercise-001/README.md) | [exercise-001/SOLUTION.md](https://github.com/graugans/tea24/blob/solutions/exercise-001/SOLUTION.md) |
| [Nr. 002](exercise-002/README.md) | [exercise-002/SOLUTION.md](https://github.com/graugans/tea24/blob/solutions/exercise-002/SOLUTION.md) |
| [Nr. 003](exercise-003/README.md) | [exercise-003/SOLUTION.md](https://github.com/graugans/tea24/blob/solutions/exercise-003/SOLUTION.md) |

Lokal ansehen und ausprobieren:

```sh
git fetch upstream
git switch -c solutions upstream/solutions   # einmalig: lokalen Branch anlegen
```

Später auf den neuesten Stand bringen:

```sh
git fetch upstream
git switch solutions
git reset --hard upstream/solutions
```

Der Branch `solutions` wird gelegentlich überarbeitet. Verwenden Sie deshalb `reset --hard` statt `pull` und machen
Sie auf diesem Branch **keine eigenen Commits**. Ihre eigenen Lösungen gehören in eigene Branches (z. B.
`solution-002`).


## Synchronisation des eignen Forks

Es wird davon ausgegangen, dass Sie einen Fork angelegt haben und von diesen geklont haben. Damit entspricht Ihr Fork der Bezeichnung ``origin``. Siehe hierzu auch die [Hinweise in der ersten Übungsaufgabe](exercise-001#upstream-hinzuf%C3%BCgen).

Um die aktuellen Änderungen in die lokale Kopie des ``tea24`` Repository zu übertragen sollten Sie zunächst ein ``git fetch`` durchführen

```sh
git fetch upstream
```

Nun checken wir die lokale Kopie des zu synchronisierenden Branches aus, in unserem Fall der ``main`` Branch. Dieser Schritt kann entfallen falls wir bereits schon auf dem ``main`` Branch sind. Das können Sie mit ``git branch -a`` prüfen.

```sh
git checkout main
```

Der Befehl ``git pull`` aktualisiert nun den lokalen Branch.

```sh
git pull upstream main
```

Sollte es hierbei zu einem Konflikt kommen haben Sie eventuell versehentlich lokale commits auf Ihrem ``main`` Branch durchgeführt. Das sollten Sie tunlichst vermeiden.

Im Erfolgsfall müssen jetzt nur noch die Daten in Ihren Fork gepusht werden:

```sh
git push origin main
```

## Übungsaufgaben

- [Übungsaufgabe Nr. 000](exercise-000/README.md)

## Hilfreiche Links

- [Markdown Cheat Sheet](https://docs.github.com/en/get-started/writing-on-github)
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf)
