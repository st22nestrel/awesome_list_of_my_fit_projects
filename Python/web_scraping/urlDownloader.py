import mechanicalsoup

mainpage = "https://www.kozmetikalacno.sk"

browser = mechanicalsoup.StatefulBrowser()
browser.open(mainpage)

sites_with_products = browser.page.select('.c1996')

urlList = set()

for link in sites_with_products:
    browser.follow_link(link.attrs['href'])

    loop = True

    while loop:
        for item in browser.page.select('.c315'):
            urlList.add(browser.absolute_url(item.attrs['href']))

        loop = browser.page.select_one('a.c74.active')
        
        if loop:
            browser.follow_link(loop)

with open('urls_demo.txt', 'w') as fp:
    fp.write('\n'.join(urlList))