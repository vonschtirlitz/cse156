travishello:
	gcc -o travishello ./travistest/travishello.c
clean:
	rm travishello
test:
	./travishello
