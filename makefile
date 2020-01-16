travishello:
	gcc -o travishello ./travistest/travishello.c
cleantravis:
	rm travishello
testtravis:
	./travishello
################
pa1:
	gcc -o pa1 ./lab1/src/main.c
cleanpa1:
	rm pa1
testpa1G:
	./pa1 www.example.com 93.184.216.34/index.html
testpa1GP:
	./pa1 www.example.com 93.184.216.34:80/index.html
testpa1H:
		./pa1 www.example.com 93.184.216.34/index.html -h
testpa1HP:
		./pa1 www.example.com 93.184.216.34:80/index.html -h
