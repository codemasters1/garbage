ferdek
radek


function measureSimilarity(word1, word2)
{
	var parts = [];

	for (var pos = 0; pos < word1.length; ++pos)
	{
		for (var len = word1.length - pos; len > 0; --len)
		{
			var sub = word1.substr(pos, len);

			if (word2.indexOf(sub) > -1)
			{
				pos += sub.length;

				parts.push(sub);
			}
		}
	}

	return parts;
}


def measureSimilarity(word1, word2):
	parts = []

	while pos < len(word1):
		while length > 0:
			sub = word1[pos:length]

		if word2.index(sub) > -1:
			pos += len(sub)
			parts.append(sub)


	return parts

function measureSimilarity(word1, word2)
{
	var parts = [];

	for (var pos1 = 0, pos2 = 0; pos1 < word1.length; ++pos1, ++pos2)
	{
		var pos_tmp = pos1;
		var pos2 = 

		for (var len = word1.length - pos1; len > 0; --len)
		{
			var sub = word1.substr(pos1, len);

			if (word2.indexOf(sub) > -1)
			{
				pos1 += sub.length;

				parts.push([sub, sub]);
			}
		}

		parts.push([]);
	}

	return parts;
}

function measureSimilarity(word1, word1)
{
	var parts = [];
	var tmp_pos = 0;

	for (var pos1 = 0; pos1 < word1.length; ++pos1) 
	{
		var found = false;

		for (var len = word1.length - pos1; len > 0; --len)
		{
			var sub = word1.substr(pos1, len);
			pos2 = word2.indexOf(sub);

			if (pos2 == -1)
			{
				continue;
			}

			found = true;
			
			pos1 += sub.length;

			parts.push([sub, sub]);
		}

		if (! found)
		{
			parts.push([]);
		}
	}
}

['a', 'b'],
['aa', 'c'],


radek ferdek
'ra'  'dek'
'fer' 'dek'


for i in range(1000, 10000, 1):
	x = [int(_) for _ in str(i).split('')]

	if (x[0] + x[1] + x[2] + x[3]) == 4:
		print(i)