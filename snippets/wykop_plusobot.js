setInterval(function() {
	var found = $('.type-light-warning.active-annotation > a');

	if (found.length == 0)
	{
		return;
	}

	var button = found.eq(0);
	button.click();

	setTimeout(function() {
		$('.button.mikro.ajax').each(function() {
			if (!$(this).prev().hasClass('voted')) {
				$(this).click();
			} else {
				$(this).parents('li.entry.iC').remove();
			}
		});
	}, 500);

}, 5000);