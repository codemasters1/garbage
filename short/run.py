import sqlite3
from flask import Flask, render_template, request
from flask import redirect
from flask import url_for

import config

app = Flask(__name__)


def convert_to_text_id(n: int) -> str:
    chars = config.ID_CHARS
    base = len(chars)
    u = []
    while n > 0:
        n, b = divmod(n, base)
        u.append(chars[b - 1])
    return ''.join(reversed(u))


def convert_to_num_id(t: str) -> int:
    chars = config.ID_CHARS
    base = len(chars)
    try:
        return sum(((chars.index(c) + 1) * base ** i for i, c in enumerate(reversed(t))))
    except ValueError:
        return 0


@app.route('/', methods=['GET'])
def index():
    return render_template('index.html')


@app.route('/', methods=['POST'])
def index_form():
    url = request.form.get('url', None)
    if url:
        with sqlite3.connect(config.DB_FILE) as db:
            c = db.execute('INSERT INTO urls(url) VALUES (?)', (url,))
        return render_template('index.html', url=config.HOME_URL.format(convert_to_text_id(c.lastrowid)))

    return redirect(url_for('index'))


@app.route('/<string:url_id>')
def url_redirect(url_id: str):
    num = convert_to_num_id(url_id)
    if num:
        with sqlite3.connect(config.DB_FILE) as db:
            row = db.execute('SELECT url FROM urls WHERE id = ?', (num,)).fetchone()
            if row:
                url, = row
                return redirect(url)

    return redirect(url_for('index'))


@app.route('/test')
def test():
    return '379837460'

if __name__ == '__main__':
    with sqlite3.connect(config.DB_FILE) as db:
        db.execute('CREATE TABLE IF NOT EXISTS urls (id INTEGER PRIMARY KEY, url TEXT)')

    app.run(debug=True)
