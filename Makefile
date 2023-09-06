# Muntean Vlad-Andrei 315CA
build:
	gcc -g tiktok.c bonus.c stream.c selections.c effects.c -o image_editor -Wall -Wextra -lm
	
clean:
	rm -f image_editor
