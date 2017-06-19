# GMZ2.1

## Gas Mess Zentrale für bis zu 2 analoge Sensoren in 4-20mA Technologie

Dies ist der `master` Branch des Projektes. Alle Dateien bilden ein buildbares Projekt.

Die verschiedenen Gas Arten sind in `git` Branches realisiert.

## Projekt mit unterschiedlichem Gas erstellen

**Als Beispiel dient der Branch C5H12-0-100UEG-linear, diesen bitte durch den gewünschten ersetzen.**

```bash
# sicherstellen das vom `master` Branch aus opperiert wird
git checkout master

# eine neuen, temporären, branch aus dem `master` erstellen
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

# Entwicklung
## ein Branch für eine neue Gasart erstellen

Um ein Branch für eine bisher noch nicht unterstützte Gasart zu erstellen, 
muss zunächste der `template` Branch ausgecheckt, dannach die `Defines.h` angepasst
und anschließend dieser Branch unter der Bezeichnung des neuen Gases ins
Remote Repository gepushed werden. Das klingt aufwending, ist aber nicht viel Arbeit.

**Als Beispiel dient diesmal das Gas `H2` (Wasserstoff) mit einem Messbereich
von 0-100% UEG und linearer Sensorauswertung.**

```bash
# template in einem branch mit der neuen Bezeichnung auschecken
git checkout template -b H2-0-100UEG-linear

# jetzt die Defines.h editieren

# Anschließend wird dieser Branch in das Remote Repo gepushed
git push --set-upstream origin H2-0-100UEG-linear

# nun kann der master Branch wieder ausgecheckt werden, von diesem werden
# alle weiteren Schritte gestartet
git checkout master
```

Nun können die Schritte von 'Projekt mit unterschiedlichem Gas erstellen' 
durchgeführt werden. Hier nocheinmal die Schritte angepasst an `H2-0-100UEG-linear`.

```bash
git checkout master
git checkout -b merge master
git checkout H2-0-100UEG-linear Defines.h
```

Nun kann kompiliert werden. 
Anschließend wird das Hex File in die Versionskontrolle aufgenommen.

Nachdem das Hex File gebuildet und dokumentiert wurde kann der 
temporäre `merge` branch wieder gelöscht werden.

```bash
git checkout master
git branch -d merge
git reset HEAD .
git checkout .
```



## Konvertierung "altes" Branch Format nach "merge" Format

Die alten Branches enthielten immer komplette MplabX Projekte. Diese Art der Versionskontrolle war extem unflexibel wenn andere Dateien außer der Defines.h geändert werden mussen. Oftmals waren wichtige Dateien wie z.B. die LCD.h anders als im `master` Branch.

Das neue Branch Format enthält für die verschiedenen Gase nun nur noch die Dateien die für das Gas anders sind, als die Dateien aus dem `master` Branch (oft einfach nur die Defines.h und eine andere README.md)

```
git checkout alter-branch-gas
git rm -rf .
git reset -- Defines.h
git checkout Defines.h
git checkout template README.md
git checkout template .gitignore
git commit -a -m "Update des Branches auf das neue 'merge' Format"
git push
```
