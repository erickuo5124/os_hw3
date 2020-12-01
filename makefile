all:
	gcc main.c -o main
clean:
	rm main
	rm *.output
	rm ./stroage/*