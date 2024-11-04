Project Name  : FLP logic project - Kostra grafu (Spanning tree)
Author        : Timotej Ponek, xponek00
Academic year : 2022/2023

Program sa prekladá cez príkaz 'make' a následne je možné spustiť už preložený súbor 'flp22-log'. (v súlade so zadaním). Program ďalej očakáva, že na vstupe sa skutočne nechádza neorientovaný graf. Týmto chcem podotknúť, že program nevydá správny výsledok, ak na vstupe sú dve zameniteľné hrany ako (A B) a (B A). Tento graf chápem už ako orientovaný, a nesprávny vstup. V zadaní sa píše iba, že máme ošetriť prípady kedy je na vstupe nesúvislý neorientovaný graf, a teda situáciu akú som popísal vyššie som neošetroval.

Pre implementáciu nájdenia všetkých kostier grafu bol použitý
algoritmus, ktorý bol v podstate prevzatý z _(Algorithms for generating all possible spanning trees of a simple undirected connected graph - https://link.springer.com/article/10.1007/s40747-018-0079-7)_. Inšpiroval som sa prvým algoritmom (Char’s algorithm), ktorý vyhľadáva všetky možné kombinácie hrán vstupného grafu, ktoré sú o dlžke |počet vrcholov - 1|. Následne pre všetky kombinácie skontroluje, či sa v nich nenachádzajú cykly (to znamená, že sa dokážem v grafe dosťať z bodu A spať do bodu A bez toho, aby som nejakú hranu prešiel 2-krát), a tie v ktorých sa cykly nenachádzajú, prehlási za kostry grafu.
Môj algoritmus funguje podobne. Najskôr vyhľadám všetky možné kombinácie hrán o dĺžke |počet vrcholov - 1|. Následne ale kontrolujem, či ide o kostry grafu iným spôsobom. Kontrolujem, či sa v každej vybranej kombinácií hrán nachádzajú všetky vrcholy, ktoré graf obsahuje (toto by malo byť ekvivalentné s hľadaním cyklu, pretože kostra obsahuje |počet vrcholov - 1| hrán, a ak nám nejaký vrchol v kostre chýba, znamená to, že kostra automaticky obsahuje cyklus (chýba nám jeden vrchol, to znamená že do nejakého vrcholu sa vieme dostať z bodu A 2 rôznymi cestami - máme cyklus)). Následné, ak nejaký vrchol chýba v kombinácií hrán, kombinácia nie je prehlásená za kostru grafu. 

Myslím si, že ďalšie fungovanie programu je jednoducho čitateľné zo zdrojového kódu, a netreba ho tu nejako rozpisovať.

Program bol testovaný s jednoduchými vstupmi. Na spracovanie vstupu používam súbor _input.pl_ a ďalej používam vstavanú knižnicu _ugraphs_, pre získanie vrcholov z hrán a kontrolu, či je graf súvislý. Pre kontrolu, či je graf súvislý, najskôr získam pomocou funkcie `transpose` graf s rovnakými vrcholmi ale prehodenými hranami (napr. pôvodná hrana A-B bude prehodená na B-A). Prečo potrebujem tento graf vytvorený funkciou `transpose` najlepšie ukážem na príklade:

vstupné pole hrán: \[a-b,b-c,a-c\]
vytvorený graf: \[a-\[b,c\], b-\[c\], c\[\]\]
transponovný graf: \[a-\[\], b-\[a\], c\[a,b\]\]

Následnú kontrolu, či je graf súvislý prevádzam tak, že kontrolujem v oboch grafoch, či je daný vrchol v aspoň jednom grafe prepojený z iným vrcholom. Ak nejaký vrchol nie je prepojený s aspoň jedným vrcholom, graf je nesúvislý a program sa ukončuje.

Kód bol dokumentovaný štýlom, popísaným v _https://people.cs.vt.edu/~ryder/5314/Projects/Prolog/SEdwardsPrologComments.pdf_.
