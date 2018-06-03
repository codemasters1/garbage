import asyncio
import random
import re
import requests
import time


def proxies():
	page = 1
	regex = re.compile('<td class="a1"><a href="(.*[^\"])">')

	while True:
		response = requests.get('http://prx.centrump2p.com/{page}'.format(page=page))
		matches = regex.finditer(response.text)
		count = 0

		for match in matches:
			count += 1
			yield match.group(1)

		if count == 0:
			break
		else:
			page += 1


async def do():
	for proxy in proxies():
		url = 'http://olx.pl/ajax/misc/contact/phone/eCehH/'
		print(proxy)

		try:
			r = await requests.post(proxy + 'includes/process.php?action=update', data=dict(u=url), headers={'referer': proxy}, timeout=2)
		except Exception as e:
			print(e)
		else:
			print('ok')
		

loop = asyncio.get_event_loop()
loop.run_until_complete(do())
loop.close()