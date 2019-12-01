all:
	rm -rf dist
	mkdir dist
	lex -o dist/lex.yy.c src/file.l
	yacc -d -o dist/y.tab.c src/file.y
	cc -o dist/file dist/y.tab.c -ll -Ly

run:
	$(MAKE) all
	./dist/file < test.txt

