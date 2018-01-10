# GMZ2.1

## Gas Mess Zentrale für bis zu 2 analoge Sensoren in 4-20mA Technologie

Dies ist der `master` Branch des Projektes. Alle Dateien zusammen bilden
das Projekt.

Die verschiedenen Gas Arten sind in so genannten `git` Branches geordnet.
In diesen Branches sind nur die Dateien enthalten die im Vergleich
zum `master` Branch unterschielich sind

## Projekt mit unterschiedlichem Gas erstellen

**Als Beispiel dient der Branch C5H12-0-100UEG-linear,
diesen bitte durch den gewünschten ersetzen.**

```bash
# sicherstellen das vom `master` Branch aus opperiert wird
git checkout master

# eine neuen, temporären, branch aus dem `master` Branch erstellen
git checkout -b merge master

# die Defines aus dem Branch importieren der als Vorlage dienen soll
git checkout C5H12-0-100UEG-linear Defines.h
```

Nun kann kompiliert werden.
Anschließend wird das Hex File in die Versionskontrolle aufgenommen.

Nachdem das Hex File gebuildet und dokumentiert wurde kann der
temporäre `merge` branch wieder gelöscht werden.

```bash
git checkout master
git branch -d merge

# Anschließend wird der master HEAD wieder zurückgesetzt
git reset HEAD .

# und die Software Version des Masterbranches wieder hergestellt
git checkout .
```
