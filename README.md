# GMZ2.1

## Gas Mess Zentrale für bis zu 2 analoge Sensoren in 4-20mA Technologie

Dieser, scheinbar leere, Branch enthält nur die Dateien die im Vergleich zum Master Branch des Repos anders sind.

Um aus diesen Daten eine lauffähige Version zu generieren sollten folgende Schritte ausgeführt werden.

```bash
# sicherstellen das vom `master` Branch aus opperiert wird
git checkout master

# eine neuen, temporären, branch aus dem `master` erstellen
git checkout -b merge master

# die Defines aus dem Branch importieren der als Vorlage dienen soll
git checkout CO-0-1000ppm-Kurve Defines.h

# nachdem das Hex File gebuildet und dokumentiert wurde kann der temp. branch
# wieder gelöscht werden
git branch -d merge

# Anschließend wird der Head wieder zurückgesetzt
git reset HEAD .

# und die Software Version des Masterbranches wieder hergesetllt
git checkout .
```

Nun kann kompilliert werden.

Zum Abschluss wird das Hex File in die Versionskontrolle aufgenommen.
