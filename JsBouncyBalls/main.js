Point = function(x, y) {
	this.x = x;
	this.y = y;
}

Point.prototype.distance = function(second) {
	var dx = this.x - second.x;
	var dy = this.y - second.y;

	return Math.sqrt(dx*dx + dy*dy);
}

Vector = function(x, y) {
	this.x = x;
	this.y = y;
}

Vector.prototype.length = function() {
	return Math.sqrt(Math.pow(this.x, 2) + Math.pow(this.y, 2));
}

Vector.prototype.reverseX = function() {
	this.x *= -1;
}

Vector.prototype.reverseY = function() {
	this.y *= -1;
}

Vector.prototype.reverse = function () {
	this.reverseX();
	this.reverseY();
}

Vector.prototype.multiply = function(n) {
	return new Vector(this.x * n, this.y * n);
}

Vector.prototype.add = function(second) {
	return new Vector(this.x + second.x, this.y + second.y);
}

Bullet = function(position, velocity, time) {
	this.velocity = velocity;
	this.time = time;
	this.radius = 20;
	this.position = position;
	this.weight = 10;
}

Bullet.prototype.updateTime = function() {
	this.time = +new Date;
}

Bullet.prototype.getCurrentPosition = function() {
	var dt = (+new Date - this.time) / 1000;
	var x = this.position.x + this.velocity.x * dt;
	var y = this.position.y + (this.velocity.y * dt - g * dt * dt / 2);

	return new Point(x, y);
}

Bullet.prototype.getCurrentVelocity = function() {
	return new Vector(this.velocity.x, this.velocity.y - g * (+new Date - this.time) / 1000);
}



var canvas = document.getElementById('chart');
var context = canvas.getContext('2d');
context.translate(0, canvas.height);
context.scale(1, -1);

const g = 10;

var dragStart = null;
var bullets = [];

function drawBullets()
{
	context.clearRect(0, 0, canvas.width, canvas.height);

	for (var i = 0; i < bullets.length; i++)
	{
		var bullet = bullets[i];
		var position = bullet.getCurrentPosition();

		context.beginPath();
  		context.fillStyle = 'green';
  		context.arc(position.x, position.y, bullet.radius, 0, 2 * Math.PI, false);

  		context.fill();
  		context.beginPath();
  		context.fillStyle = 'white';
  		context.font = '20px Arial';
  		context.fillText(i, position.x, position.y);
  		context.fill();
	}
}

function detectCollisions()
{
	var collisions = {};
	for (var i = 0; i < bullets.length; i++)
	{
		var bullet = bullets[i];
		var position = bullet.getCurrentPosition();

		if (position.x + bullet.radius >= canvas.width || position.x <= bullet.radius)
		{
			bullet.position = bullet.getCurrentPosition();
			bullet.velocity = bullet.getCurrentVelocity();
			bullet.velocity.reverseX();
			bullet.velocity.multiply(0.5);
			bullet.updateTime();
		}

		if (position.y + bullet.radius >= canvas.height || position.y <= bullet.radius)
		{
			bullet.position = bullet.getCurrentPosition();
			bullet.velocity = bullet.getCurrentVelocity();
			bullet.velocity.reverseY();
			bullet.velocity.multiply(0.5);
			bullet.updateTime();
		}

		// Collisions between bullets
		

		for (var j = i + 1; j < bullets.length; j++)
		{
			var bullet2 = bullets[j];

			if (bullet.position.distance(bullet2.position) <= bullet.radius + bullet2.radius)
			{
				// var momentum = bullet.getCurrentVelocity().multiply(bullet.weight).add(bullet2.getCurrentVelocity().multiply(bullet2.weight));
				// var velocity = momentum.multiply(1/(bullet.weight+bullet2.weight));
				
				// bullet.position = bullet.getCurrentPosition();
				// bullet.velocity = velocity;
				// bullet.velocity.reverse();
				// bullet.updateTime();

				// bullet2.position = bullet2.getCurrentPosition();
				// bullet2.velocity = velocity;
				// bullet2.velocity.reverse();
				// bullet2.updateTime();

				// modified.push(i);
				// modified.push(j);
				if (collisions[i] == undefined) {
					collisions[i] = [];
				}
				collisions[i].push(j);
			}
		}
	}


	var collisionGroups = [];
	for (var i in collisions) {
		collisionGroups.push([i].concat(collisions[i]));
	}

	console.log(collisionGroups);
}

canvas.addEventListener('mousedown', function(e) {
	dragStart = new Point(e.offsetX, e.offsetY);
});

canvas.addEventListener('mouseup', function(e) {
	if (dragStart != null)
	{
		dragStop = new Point(e.offsetX, e.offsetY);
		bullets.push(new Bullet(new Point(dragStop.x, canvas.height-dragStop.y), new Vector(dragStart.x - dragStop.x, dragStop.y - dragStart.y), +new Date));
	}
});

canvas.addEventListener('mousemove', function(e) {
	document.getElementById('coords').innerText = '(' + e.offsetX + ', ' + e.offsetY + ')';
});

setInterval(function() {
	detectCollisions();
	drawBullets();
}, 5);