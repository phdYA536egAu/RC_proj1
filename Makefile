e:
	echo | nc a 0  > report.html
	cat report.html
	mv report.html /reports
	rm report.html



