var webpage = require('webpage');

var p = webpage.create();
p.open('https://www.facebook.com/', function(){
	p.render('t.png');
	for (var i in p)
	{
		console.log(i + '=' + p[i]);
	}
	phantom.exit();
});