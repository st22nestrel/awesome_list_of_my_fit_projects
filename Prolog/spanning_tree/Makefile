# Project Name  : FLP logic project - Kostra grafu (Spanning tree)
# Author        : Timotej Ponek, xponek00
# Academic year : 2022/2023


PROG = flp22-log

all: $(PROG)

$(PROG):
	swipl -q --goal=main --stand_alone=true -o $@ -c src/proj.pl

clean:
	rm -f $(PROG)
