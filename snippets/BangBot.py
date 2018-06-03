from io import BytesIO
import time
import bs4
import requests
import pytesseract
from PIL import Image


def get_captcha(session):
    image_data = session.get('https://www.banggood.com/index.php?com=imagecode&t=getcode').content

    image_buffer = BytesIO()
    image_buffer.write(image_data)

    return Image.open(image_buffer)


def recognise_captcha(image):
    # Remove borders
    width, height = image.size
    image = image.crop((1, 1, width - 1, height - 1))

    return pytesseract.image_to_string(image, config='-psm 8 bg')


def captcha_resolve(session):
    captcha = get_captcha(session)
    return recognise_captcha(captcha)


def process_account(email, password):
    session = requests.session()

    # Get login page form
    response = session.get('https://www.banggood.com/login.html')
    soup = BeautifulSoup(response.text, 'html.parser')
    is_logged = soup.find('div', dict(class_='user_log', haslogin='1')) is not None

    if not is_logged:
        request_id = soup.find('input', dict(type='hidden', name='at'))['value']

        while True:
            captcha_code = captcha_resolve(session)

            # Prepare form data do send
            data = dict(
                 com='account',
                 t='submitLogin',
                 at=request_id,
                 email=email,
                 pwd=password,
                 login_image_code=captcha_code
            )

            response = session.post('https://www.banggood.com/index.php', data)

            try:
                data = response.json()

                if 'success' in data:
                    print('Logged successfully!')
                    break
                elif 'target' in data:
                    print('Error:', data['msg'])

                    for left in range(5, 0, -1):
                        print('Trying again in {0} seconds...'.format(left), end='\r')
                        time.sleep(1)

                    print('Trying again...                    ')

            except ValueError as e:
                pass

    else:
        print('Already logged in.')

    response = session.get('http://www.banggood.com/index.php?com=event&t=getAnniverSignInfo')
    data = response.json()

    chances = data['todayLotteryTimes']
    coins = data['gold']

    print('Stats: {0} chances, {1} coins'.format(chances, coins))


def main():
    with open('/x/pass.txt', 'r') as f:
        for line in f:
            line = line.strip()
            if line:
                email, password = line.split('|')

                print('Processing {0}'.format(email))
                process_account(email, password)
                time.sleep(10)


if __name__ == '__main__':
    main()
