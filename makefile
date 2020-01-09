travishello:
	gcc -o travishello ./travistest/travishello.c
lab1:
	gcc -o lab1 ./lab1/src/main.c
cleantravis:
	rm travishello
testtravis:
	./travishello
