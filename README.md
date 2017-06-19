# GMZ2.1

## Gas Mess Zentrale für bis zu 2 analoge Sensoren in 4-20mA Technologie

Dieser, scheinbar leere, Branch enthält nur die Dateien die im Vergleich zum Master Branch des Repos anders sind.

Um aus diesen Daten eine lauffähige Version zu generieren sollten folgende Schritte ausgeführt werden.

**Als Beispiel dient der Branch C5H12-0-100UEG-linear, diesen bitte durch den gewünschten ersetzen.**

```bash
# sicherstellen das vom `master` Branch aus opperiert wird
git checkout master

# eine neuen, temporären, branch aus dem `master` erstellen
git checkout -b merge master

# die Defines aus dem Branch importieren der als Vorlage dienen soll
git checkout C5H12-0-100UEG-linear Defines.h

# nachdem das Hex File gebuildet und dokumentiert wurde kann der temp. branch
# wieder gelöscht werden
git checkout master
git branch -d merge

# Anschließend wird der Head wieder zurückgesetzt
git reset HEAD .

# und die Software Version des Masterbranches wieder hergesetllt
git checkout .
```

Nun kann kompilliert werden.

Zum Abschluss wird das Hex File in die Versionskontrolle aufgenommen.
