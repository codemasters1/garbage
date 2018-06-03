from flask import Flask, redirect, render_template, send_from_directory
from bs4 import BeautifulSoup
import requests

app = Flask(__name__)


@app.route('/assets/<path:path>')
def send_asset(path):
    return send_from_directory('static', path)


@app.route('/random')
def random():
    while True:
        response = requests.get('http://gfycatporn.com/60fps.php')
        soup = BeautifulSoup(response.content, 'html.parser')

        gfy_id = soup.find('div', class_='gfyitem').get('data_id')
        gfy_url = 'http://giant.gfycat.com/{0}.webm'.format(gfy_id)

        response = requests.head(gfy_url)
        if response.status_code == 200:
            return gfy_url


@app.route('/')
def index():

    return render_template('index.html', gfy_url='a')

if __name__ == '__main__':
    app.run(debug=True, host='spoder.pl', port=8080)
