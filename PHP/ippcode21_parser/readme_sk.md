Implementační dokumentace k 1. úloze do IPP 2020/2021
Jméno a příjmení: Timotej Ponek
Login: xponek00
## Analýza kódu
**checkHeader** - kontroluje prítomnosť hlavičky. **checkInstructions** - kontroluje riadky zo STDIN, vždy rozdelí riadok na dve časti podľa znaku '#', ďalej prvú časť rozdelí podľa medzier a podľa zhody prvého prvku z výsledného poľa vo switch konštrukcii sa kontroluje daná inštrukcia. Inštrukcie sú rozdelené do skupín podľa počtu a typu operandov, až na `LABEL` ktorá je zvlášť kvôli správnemu počítaniu skokov v rozšírení STATP.

**checkVar, checkSymb, checkLabel** funkcie zodpovedajú svojim názvom. Kontrolujú či premenná, symbol alebo náveštie zodpovedá regulárnemu výrazu, ktorý zodpovedá definícií typov zo zadania.
# Rozšírenie STATP
## Trieda Files
Zapuzdruje názvy súborov a jednotlivé kategórie štatistík ktoré sa majú do nich vypísať, a metódy na prácu s nimi.
## Trieda Stats
Zbiera jednotlivé štatistiky o spracovanom súbore. Počet komentárov sa zvyšuje s ich výskytom pri analýze súboru, linesOfCode je definovaný ako `order - 1`. Všetky ostatné štatistiky sa nastavujú cez metódy. Náveštie sa pridáva cez metódu **addLabel** - skontroluje či kľúč s názvom náveštia existuje v poli `NotDefinedLabels` ktoré reprezentuje náveštia na ktoré sa skáče ale zatiaľ neboli v súbore definované. Ak existuje, hodnota z tohto poľa sa pripočíta k `forwardJumps` a kľuč v poli sa zmaže, a ďalej sa vždy v poli `NamedLabels` vytvorí kľúč *názov náveštia <-> riadok na ktorom sa vyskytlo*.

**checkJump** - skontroluje či sa kľúč *názov náveštia* nachádza v `NamedLabels`, ak nie tak ho tam vloží so zápornou hodnotou riadku, je to tak preto aby sa pri ďalšom volaní nekontrolovala prítomnosť kľúča v oboch poliach, a v `NotDefinedLabels` sa vytvorí kľúč s hodnotou 1. Ak sa v `NamedLabels` kľúč nachádza a nemá zápornú hodnotu, inkrementuje sa `backJumps`, inak sa inkrementuje hodnota s rovnakým kľúčom v poli `NotDefinedLabels`. 

Na konci analýzy súboru sa zo všetkých hodnôt v poli `NotDefinedLabels` vyhodnotí počet `badJumps` -> skokov na nedefinované náveštia.
### Problémy
Ak sa v kóde na vstupe vyskytne 2 a viac krát duplicitné náveštie, je počítané zvlášť pre každý výskyt a skok na toto náveštie sa počíta iba raz za všetky duplikáty. Pre vstup ktorý následne zlyhá v interprete sa jednoducho nedajú očakávať validné výsledky. Môžu nastať aj ďalšie sporné situácie, ale žiadne iné som neotestoval.
