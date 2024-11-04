# Web scraping

## About

This project gathers urls of all the products listed on the website 'https://www.kozmetikalacno.sk/' - script `urlDownloader.py`. Later, data about these products like product name and price can be obtained from the urls and stored in `.tsv` format - script `tsvDownloader.py`

### School subject and project

-   *subject*: **UPA - Data Storage and Preparation**
-   *project*: Project, part 3: extracting data from the web (Projekt, 3. část: extrakce dat z webu)

## Usage

### Prerequisites

- `python3` (implemented with `python3.8`)
- python library `mechanicalsoup` -> run `./build.sh` to install it
- OPTIONAL: you can also install `mechanicalsoup` in virtual enviroment - [manual](../../0-manuals/venv_manual.md). But this project has no `requirements.txt`, so you have to specify the library to install - `pip3.8 install mechanicalsoup`.

### How to run

```bash
./run.sh
```

Running the above will download urls of all the products (these will be stored in `urls.txt`) and then collect data about the name and price of the products (stored in `data.tsv`). The resulting output format is **\<url\>tab\<name\>tab\<price\>**