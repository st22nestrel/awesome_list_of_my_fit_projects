# KRY project 2 - Hybrid encryption
# Timotej Ponek, xponek00
.PHONY: build run

build:
	python3 -m venv venv
	. venv/bin/activate && pip install -r requirements.txt

run:
	. venv/bin/activate && python kry.py $(TYPE) $(PORT)

clean:
	rm -rf venv cert __pycache__