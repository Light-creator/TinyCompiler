parser = \
	parser/parser.c

lexer = \
	lexer/lexer.c \
	token/token.c

utils = \
	utils/string_view.c

SRCS = \
	main.c  \
	$(lexer) \
	$(utils) \
	$(parser)

build:
	gcc $(SRCS) -o main

clean:
	rm -rf ./main

all: clean build

run:
	./main

asm:
	nasm -f elf64 -o main.o main.asm
	ld -o main.out main.o