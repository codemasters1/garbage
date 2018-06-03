const LOGIN = 'changeme';
const PASSWORD = 'changeme';

if (LOGIN == 'changeme' || PASSWORD == 'changeme')
{
	console.log('Change login and password.');
}


var webpage = require('webpage');
var system = require('system');

var page = webpage.create();

page.settings.userAgent = 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/39.0.2171.99 Safari/537.36';
page.viewportSize = {
  	width: 1366,
  	height: 600
};

console.log('Connecting and downloading website data...');

page.open('https://primedice.com/welcome', function() {
	console.log('Main page loaded.');

	page.render('glowna.png');

	var is_already_logged = page.evaluate(function() {
		var login_link = document.querySelector('.login');

		if (login_link == null) {
			return true;
		} else {
			document.querySelector('.login').click();

			return false
		}
	});

	if (is_already_logged) {
		console.log('Already logged in.')
	} else {
		console.log('Logging in...');
		setTimeout(function() {
			page.evaluate(function() {
				document.querySelector('#loginForm [name=username]').focus();
			});

			page.sendEvent('keypress', LOGIN);
			page.sendEvent('keypress', page.event.key.Tab);
			page.sendEvent('keypress', PASSWORD);
			page.sendEvent('keypress', page.event.key.Enter);

			page.render('logowanie.png');

		}, 500);
	}
});

page.onLoadFinished = function(status) {
  	console.log('Loading ' + page.url + ': ' + status);

  	var url = page.evaluate(function() {
  		return location.href;
  	});

  	console.log('Current url: ' + url);

  	if (url == 'https://primedice.com/play') {
		page.evaluate(function() {
      		document.querySelector('.icon--balance').click();
    	});

    	console.log('Waiting for captcha...');

		var waiter = function() {
			var temp_clipRect = page.clipRect;
    		var clipRect = page.evaluate(function() {
    			var captcha = document.querySelector('#adcopy-puzzle-image-image');

    			if (captcha == null) {
    				return null;
    			} else {
    				if (captcha.tagName == 'IMG') {
    					if (! captcha.complete) {
    						return null;
    					}
    				}
    			}

    			var rect = captcha.getBoundingClientRect();

    			console.log('BoundingCLientRect: ' + JSON.stringify(rect));

    			return {
					top:    rect.top + 650,
					left:   rect.left,
					width:  rect.width,
					height: rect.height
				};
    		});

    		if (clipRect == null || clipRect == '') {
				setTimeout(waiter, 500);
    		} else {
    			page.render('captcha_whole_site.png');

    			console.log('Got captcha. Saving...');
				page.clipRect = clipRect;
				page.render('captcha.png');
				
				page.clipRect = temp_clipRect;

				system.stdout.write('Enter the captcha code: ');
				var code = system.stdin.readLine();

				page.evaluate(function() {
		      		document.querySelector('#adcopy_response').focus();
		    	});

				page.sendEvent('keypress', code);

				page.render('captche_entered.png');

    		}
    	}

    	waiter();
  	}
};

