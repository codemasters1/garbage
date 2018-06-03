import os
import bs4
import requests

DIRECTORY = 'books'
BOOKS_URL = 'http://www.oreilly.com/programming/free/'
BOOK_URL = 'http://www.oreilly.com/programming/free/files/{id}.{format}'


def find_books():
    response = requests.get(BOOKS_URL)
    doc = bs4.BeautifulSoup(response.content, 'html.parser')

    links = doc.find_all('a', {'data-toggle': 'popover'})
    for link in links:
        title = link.get('title')
        url = link.get('href')
        book_id = url.rsplit('/', 1)[1].split('.', 1)[0]

        yield title, book_id


def download_file(url, path):
    response = requests.get(url, stream=True)
    with open(path, 'wb') as f:
        for chunk in response.iter_content(chunk_size=4096):
            if chunk:
                f.write(chunk)

    return True

if __name__ == '__main__':
    if not os.path.exists(DIRECTORY):
        os.makedirs(DIRECTORY)

    for title, book_id in find_books():
        for fmt in 'mobi epub pdf'.split():
            book_url = BOOK_URL.format(id=book_id, format=fmt)
            book_file = '{dir}/{name}.{ext}'.format(dir=DIRECTORY, name=book_id, ext=fmt)

            if not os.path.exists(book_file):
                print('Downloading {0}'.format(title))
                download_file(book_url, book_file)
