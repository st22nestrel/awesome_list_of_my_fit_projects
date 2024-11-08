Implementační dokumentace k 2. úloze do IPP 2020/2021
Jméno a příjmení: Timotej Ponek
Login: xponek00

## Analýza XML súboru - `parsePy.py`

Vstupný XML súbor sa načíta pomocou `xml.etree.ElementTree` knižnice. Ako prvá sa skontroluje hlavička, ďalej sa zoradia inštrukcie podľa atribútu **order** a pokračuje sa syntaktickou analýzou všetkých inštrukcií. Tá prebieha skrz fukcie definované v `parsePy.py`. Okrem týchto kontrol sa tu v reťazcoch prevádzajú escape sekvencie na zodpovedajúce znaky, inštrukcie sú vždy prevedené do uppercase aby sa vyhlo errorom pri interpretácií a ukladajú sa tú všetky definované návestia do dictu.

## Interpretácia kódu - `interpret.py`

Kód sa interpretuje prechádzaním zoradených inštrukcií v liste od prvého prvku. Pre každú inštrukciu je definovaná samostatná funkcia, kde sa získajú potrebné argumenty pre prevedenie inštrukcie a ďalej nasleduje samotné vykonanie inštrukcie. Po každej prevedenej inštrukcii sa inkrementuje počítadlo **order**, to určuje ktorá inštrukcia sa z listu vyberie ako ďalšia. Pri skokových inštrukciách sa v dicte **labelStack** nájde číslo inštrukcie, v ktorej bolo definované náveštie a skáče sa na túto inštrukciu.

Vypichol by som ešte pomocnú funkciu **getVarORSymbVals**, ktorá na základe parametru **dataType**, ak ide o premennú, vráti jej hodnotu zo zodpovedajúceho rámca ak je definovaná, a ak ide o hocijakú konštantu, prevedie jej textový zápis na zodpovedajúci datový typ.

## Reprezentácia datových typov jazyka IPPcode21 - `dataStructs.py`
Jednotlivé datové rámce sú reprezentované štruktúrou binárneho stromu. Do položky stromu sa ukladá názov premennej, datový typ reprezentovaný enumom a hodnota. Nad stromom sú definované operácie **insert** (vloženie neinicializovanej premennej), **actualize** a **search** (hľadanie premennej a vrátenie jej datového typu a hodnoty v prípade nájdenia). Zásobník je jednoducho reprezentovaný dvojicou listov, ktoré sú spojené v štruktúre **Stack**. Jednotlivé operácie nad zásobníkom potom prebiehajú pridávaním a odoberaním položiek odzadu z oboch listov. Na prevod textového zápisu jednotlivých dát z XML súboru sa používa funkcia **toType**.

Lokálne rámce sa pri inštrukcii PUSHFRAME ukladajú do listu s **frames** a pri inštrukcii POPFRAME sa rámec vyberie a uloží do premennej **localSymbols** (tento zásobník funguje na rovnakom princípe ako štruktúra **Stack**) 

## Testovací skript `test.php`

Po spracovaní argumentov skript vyhľadá len súbory s príponou *".src"* pomocou rekuzívnych iterátorov z php knižnice. Tieto názvy súborov sa uložia do poľa **LSfiles** cez ktoré sa následne iteruje a vykonávajú sa vybrané testy.

Každý test sa začína otvorením, prípadne vytvorením testovacích súborov s koncovkou *".in"*, *".out"* a *".rc"* s názvom rovnakým ako názov súboru s príponou *".src"*. Pre každý test sa vytvára dočasný súbor do ktorého sa ukladajú výstupy jednotlivých testovaných skriptov a po vyhodnotení testu je tento súbor odstránený.

Jednotlivé výsledky sa hneď vypisujú na výstup v podobe jednoduchej HTML tabuľky. V súbore `testArgs.php` sú definované triedy **testArgs**, ktorá zapuzdruje jednotlivé spúšťacie argumenty skriptu, a **HTMLSupport,** ktorá ráta počet úspešných a neuspešných testov a poskytuje funkcie na tvorbu tabuľky s výsledkami.