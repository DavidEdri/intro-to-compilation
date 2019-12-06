all:
	rm -rf dist
	mkdir dist
	cp -r src/utils dist/utils
	lex -o dist/lex.yy.c src/file.l
	yacc -d -v -o dist/y.tab.c src/file.y
	cc -o dist/file dist/y.tab.c -ll -Ly

run:
	$(MAKE) all
	./dist/file < input.c

