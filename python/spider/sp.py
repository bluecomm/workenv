import urllib.request
from bs4 import BeautifulSoup
import os
 
def Download(url,picAlt,name):
    path = '~/tmp/'+picAlt+'/'
    if not os.path.exists(path):
        os.makedirs(path)
    urllib.request.urlretrieve( url, '{0}{1}.jpg'.format(path, name)) 
 
header = {
    "User-Agent":'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36',
    'Accept': '*/*',
    'Accept-Language': 'en-US,en;q=0.8',
    'Cache-Control': 'max-age=0',
    'Connection': 'keep-alive'
    }

def run(targetUrl, beginNUM ,endNUM):
    req = urllib.request.Request(url=targetUrl,headers=header)
    response = urllib.request.urlopen(req)
    html = response.read().decode('gb2312','ignore')
    soup = BeautifulSoup(html, 'html.parser')
    Divs = soup.find_all('div',attrs={'id':'big-pic' })
    nowpage = soup.find('span',attrs={'class':'nowpage'}).get_text()
    totalpage= soup.find('span',attrs={'class':'totalpage'}).get_text()
    if beginNUM ==endNUM :
        return
    for div in Divs:
        beginNUM = beginNUM+1
 
        if div.find("a") is None :
            print("No Next One")
            return
        elif div.find("a")['href'] is None or div.find("a")['href']=="":
            print("No Next One None")
            return
        print("Download INFO: progress ",beginNUM,"/",endNUM,"now download: ",nowpage,"/",totalpage,")")
 
        if int(nowpage)<int(totalpage):
            nextPageLink ="http://www.mmonly.cc/mmtp/qcmn/" +(div.find('a')['href'])
        elif int(nowpage)==int(totalpage):
            nextPageLink = (div.find('a')['href'])
 
        picLink = (div.find('a').find('img')['src'])
        picAlt = (div.find('a').find('img'))['alt']
        print('piclink',picLink)
        print('picname[ ', picAlt , ' ] ')
        print('begin downloading...........')
        Download(picLink,picAlt, nowpage)
        print("download success")
        print('next page url: ',nextPageLink)
        run(nextPageLink,beginNUM ,endNUM)
        return
 
if __name__ == '__main__':
    targetUrl ="http://www.mmonly.cc/mmtp/qcmn/237269.html"
    run(targetUrl,beginNUM=0,endNUM=70)
    print(" OVER")
