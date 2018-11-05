import requests
import selenium

res = requests.get('http://www.baidu.com')
print(res.status_code)
print(res.text)

header_info={'User-Agent':'Mozilla/5.0 (Windows NT 6.1; WOW64; rv:38.0) Gecko/20100101 Firefox/38.0'}
res = requests.get('https://www.whzh-cw.com/chapter.html?2#mybookid=49587&bookid=53367&chapterid=23544203',
        headers=header_info, timeout=5)
print(res.status_code)
print(res.text)
