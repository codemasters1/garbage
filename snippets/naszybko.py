from flask import Flask, request

app = Flask('HelloWorldApp')

@app.route('/')
def index():
	return """
		<form action="/" method="POST">
				<input name="msg">
				<input type="submit" value="OK">
		</form>
	"""

@app.route('/', methods=['POST'])
def info():
	return 'siema {0}'.format(request.form.get('msg', ''))

if __name__ == '__main__':
	app.run(debug=True)