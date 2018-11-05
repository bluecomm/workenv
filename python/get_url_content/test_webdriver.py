from selenium import webdriver
from bs4 import BeautifulSoup
import time

options = webdriver.ChromeOptions()
options.add_argument('--headless')
options.add_argument('--disable-gpu')
options.add_argument('--no-sandbox')
#driver.get("https://www.baidu.com")

# get url list
url_list = []
for url in open("b.txt.url.1-195"):
    url_list.append(url.strip('\r').strip('\n'))

global indx
indx = 0

while indx < len(url_list):
    url_current = url_list[indx]
    #print(url_current)
    #print(indx)
    try:
        driver = webdriver.Chrome(executable_path="./chromedriver", chrome_options=options)
        driver.get(url_current)
        #print(driver.page_source)
        html = driver.page_source
        #print(html)

        '''
        with open('html.txt') as fp:
            html = fp.read()
        html = html.replace('<br />','\n').replace('&nbsp', ' ')
        '''
        html = html.replace('<br />','\n').replace('&nbsp', ' ')
        html=BeautifulSoup(html,'html.parser')

        indx_begin = 1
        #indx_begin = 196
        for li in html.select('.chaptername'):
            chapter_name = '第' +  str(indx + indx_begin) +'章' + ' Chapter' + str(indx + indx_begin) + li.get_text()
            print(chapter_name)
        print()

        for br in html.find_all('div', id='txt'):
            print(br.get_text())
        print()

        driver.quit()
        time.sleep(10)
        indx += 1
    except Exception as e:
        #print('Error Occurred', e.args)
        time.sleep(20)

