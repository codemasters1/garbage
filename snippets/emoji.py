import re
import requests

def find_emojis(query):
    headers = {
        'Origin': 'http://emoji.codes',
        'Accept-Encoding': 'gzip, deflate',
        'Accept-Language': 'pl,en;q=0.8',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/55.0.2883.87 Safari/537.36',
        'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8',
        'Accept': '*/*',
        'Referer': 'http://emoji.codes/',
        'X-Requested-With': 'XMLHttpRequest',
        'Connection': 'keep-alive',
    }

    data = {
      'c': 'people',
      'q': query,
      'v': 'emojione',
      'a': 'ajax'
    }

    req = requests.post('http://emoji.codes/', headers=headers, data=data)
    content = req.content.decode('utf-8')

    pattern = r'a\sid=\"e1\-([^\"]+)\"'
    matches = re.findall(pattern, content)

    return [chr(int(x, 16)) for x in matches]

if __name__ == '__main__':
    print(find_emojis('poop'))
