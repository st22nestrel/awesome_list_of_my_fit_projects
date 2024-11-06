isa-tftp client
Vypracoval: Timotej Ponek, xponek00

Program je potrebné spúšťať cez sudo

Read request pre súbory iné ako textové v módu 'netascii' nespracuváva získane súbory správne, 
Write requst funguje očakávane - bol testovaný write .jpg a .pdf súborov.
Implementácia multicastu nebola dostatočne otestovaná, s istotu funguje keď je klient od začiatku master.

odmietnutie -t a -s parametrov:
Klient je implementovaný tak že vždy prijme všetky optiony ponúkané serverom, takže keď server odmietne hociktorý z optionov -t a -s,
tie sa jednoducho nepoužijú a prenos pokračuje s defaultnými hodnotami.

Odovzdané súbory:
args.cpp
args.h
datablocks.cpp
datablocks.h
main.cpp
Makefile
tftp.cpp
tftp.h
utils.cpp
utils.h
README
manual.pdf
