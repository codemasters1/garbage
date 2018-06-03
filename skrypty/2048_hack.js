var data = [
	[0, 0, 0, 0],
	[0, 0, 0, 0],
	[0, 0, 0, 0],
	[0, 0, 0, 0]
];

function getCol(i)
{
	return data[i];
}

function getRow(i)
{
	return [data[0][i], data[1][i], data[2][i], data[3][i]];
}

function clear(arr)
{
	var out = [];
	
	for (var i in arr)
	{
		if (arr[i] != 0)
		{
			out.push(arr[i]);
		}
	}
	
	return out;
}

function countInArray(arr)
{
	// 
}

var tiles = document.querySelector('.tile-container').childNodes;
for (var i in tiles)
{
	var tile = tiles[i];
	if (typeof tile.className == 'undefined')
	{
		continue;
	}
	
	var classes = tile.className;
	var num = classes.match(/tile-(\d+)/)[1];
	var pos = classes.match(/tile-position-(\d+)-(\d+)/).slice(1, 3);
	data[pos[0] - 1][pos[1] - 1] = parseInt(num);
}