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
testpa1:
	./pa1
