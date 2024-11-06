ipk-l2l3-scan

Program je potrebné spúšťať cez sudo, aby program mohol vytvárať sokety na ktoré potrebuje root privilégia.
Program skenuje ip adresy zadané získané z parametru -s/--subnet, a vypisuje ich dostupnosť na stdout.

V programe nie je implementované skenovanie IPv6 adries, v prípade zadania IPv6 subnetu sa iba vypíše rozsah
ip adries, a skenovanie sa preskočí. Čakanie na odpoveď pre jednotlivé skenovanie IP adresy môže
trvať dlhšie ako je nastavený parameter wait, nastavuje sa totiž zvlášť pre ARP a ICMP ping,
ale nemal bby to byť problém pretože kladná odpoveď na  ARP request (MAC adresa), príde zvačšia hneď, 
a ICMP echo sa posiela iba ak ARP získal MAC adresu, takže vačší čas čakania by sa nemal prejaviť.

Odovzdaná dokumentácia (manual.pdf) neobsahuje dostatočné uvedenie do problematiky 
a vysvetlenie návrhu riešenia.

Odovzdané súbory:
args.cpp
args.h
hosts.cpp
hosts.h
ipk-l2l3-scan.cpp
IPs.cpp
IPs.h
Makefile
timer.cpp
timer.h
README
manual.pdf
