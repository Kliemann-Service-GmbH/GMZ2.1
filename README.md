# GMZ2.1

## Gas Mess Zentrale für bis zu 2 analoge Sensoren in 4-20mA Technologie

Dies ist der Master Branch des Projektes. Alle Dateien bilden ein Buildbares Projekt.

Die verschiedenen Gas Arten sind in git Branches realisiert.

## Projekt mit unterschiedlichem Gas erstellen

```bash
# sicherstellen das vom master Branch aus opperiert wird
git checkout master

# eine neuen, temporären, branch aus dem master erstellen
git checkout -b merge master

# die Defines aus dem Branch importieren der als Vorlage dienen soll
git checkout Defines.h C5H12-0-100UEG-linear

# nachdem das Hex File gebuildet und dokumentiert wurde kann der temp. branch
# wieder gelöscht werden
git branch -d merge
```

Nun kann kompelliert werden.

Zum Abschluss wird das Hex File in die Versionskontrolle aufgenommen.


# Entwicklung
## Konvertierung "altes" Branch Format nach "merge" Format

Die Alten Branches enthielten immer komplette MplabX Projekte. Diese Art der Versionskontrolle war extem unflexibel wenn andere Dateien auser der Defines.h geändert werden mussen. Oftmals waren wichtige Dateien wie die LCD.h anders als im master Branch.

Das Neue Branch Format enthält für die verschiedenen Gase nun nur noch die Dateien die für das Gas anders sind als die Dateien aus dem Master Branch (oft einfach nur die Defines.h und eine andere README.md)

```
git checkout alter-branch-gas
git rm -rf .
git reset -- Defines.h
git checkout Defines.h
git checkout template README.md
git checkout template .gitignore
git commit -a
git push
```
