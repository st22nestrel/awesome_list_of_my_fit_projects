import mechanicalsoup
import argparse
import re
import sys

parser = argparse.ArgumentParser(description='Interpret of IPPcode21')
parser.add_argument('--file', nargs='?', metavar='file',
                   help='Input file with urls')

args = parser.parse_args()

browser = mechanicalsoup.StatefulBrowser()

def process_URL(urlProduct, fp):
    browser.open(urlProduct)
    fp.write(urlProduct + '\t')

    product_name = browser.page.select_one('.c1335')
    product_name = product_name.contents[0].strip() + '\t'
    fp.write(product_name)

    price = browser.page.select_one('.c2009')
    search_res = re.search(r"\d+,\d+", price.contents[0]).group()
    search_res = search_res.replace(",", ".")
    search_res += '\n'
    fp.write(search_res)

if(args.file):
    with open('urls.txt', 'r') as fp_url:
        with open('data.tsv', 'w') as fp:
            for urlProduct in fp_url.readlines():
                process_URL(urlProduct.strip(), fp)
else:
    with open('data_demo.tsv', 'w') as fp:
        for urlProduct in sys.stdin.readlines():
            process_URL(urlProduct.strip(), fp)