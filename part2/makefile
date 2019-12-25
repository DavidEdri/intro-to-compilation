all:
	@rm -rf dist
	@mkdir dist
	@cp -r src/utils dist/utils
	@lex -o dist/lex.yy.c src/scanner.l
	@yacc -d -v -o dist/y.tab.c src/parser.y
	@cc -o dist/output dist/y.tab.c -ll -Ly

run:
	$(MAKE) all
	@./dist/output < input.code

