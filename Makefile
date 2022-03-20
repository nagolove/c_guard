debug : 
	gcc -o ./bin/guard_check  ./src/main.c \
	-Wall -Wextra -Wpedantic -std=c18 \
	-fPIC \
	-g3 \
	-lluajit-5.1 \
	-I /usr/include/lua5.1/ \

release :
	gcc -o ./bin/res ./src/main.c -O2 -std=c18

dl :
	-rm -rf ./bin
