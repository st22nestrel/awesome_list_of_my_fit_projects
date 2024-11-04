#!/bin/bash
python3.8 urlDownloader.py

head -n 20 urls_demo.txt | python3.8 tsvDownloader.py