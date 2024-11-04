FLP - projekt 1, varianta ECDSA
Autor - Timotej Ponek, xponek00

Boli implementované všetky požiadavky zo zadania. Program bol implementovaný pre
preklad prekladačom ghc 9.2.5. Pre preklad u mňa lokálne je potrebný flag 
'-package parsec', pretože mám nesprávne lokálne nainštalovaný parsec, a ghc ho
bez tohto prepínača nevidí. Ak bude pri preklade nejaký problém s týmto flagom, 
prosím o jeho odstránenie z 'Makefile'.


Testy v zložke test: výstupy (*.out) by mali byť brané iba ako ukážkové výstupy, 
keďže pre argumenty '-k' a '-s' sa používa generátor náhodných čísel, na vygenerovanie
súkromného kľúča, čo znamená, že výstup bude pri každom behu programu iný. Výstupy pre
argument '-v' by ale mali odpovedať výstupu programu pri spustení zodpovedajúceho testu.

V module Parser sú použité niektoré konštrukcie čisto na to aby sa potlačili warningy,
pri preklade s -Wall. Mám na mysli konštrukcie typu (_ <- oneOf [':']), kde sa explicitne
zahadzuje hodnota operácie pre spokojnosť prekladača, hoci táto konštrukcia by mohla byť
zapísaná elegantnejšie spôsobom (oneOf [':']). 
V module DataOps sú zase použité konštrukcie typu (x*x) namiesto významovo jasnejšieho
(x^2), znovu kvôli warningom od prekladača. Tento warning by sa dal potlačiť zapísaním
konštrukcie ako (x^(2::Integer)), tento zápis mi ale príde zbytočne dlhý, takže som ostal
pri (x*x).