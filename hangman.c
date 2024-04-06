#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define WORD_POOL 84092
#define MAX_WRONG 6

FILE* fpin;

int get_guess(char* answer, char* word, char* guesses, int *wrong);
void show_gallows(char* guesses, char* word, int wrong);
int choose_word(char* answer);

int main()
{
	int i, wrong = 0;
	char word[50], answer[50], wrong_guesses[10], guesses[27];
	char c;
	time_t t;
	
	guesses[0] = 0;
	
	if ((fpin = fopen("engmix.txt", "r")) == NULL) {
		printf("Unable to open input file. Exiting...\n");
		exit(-1);
	}
	
	printf("Welcome to Hangman.\n");
	printf("Selecting word...\n");
	
	//Call choose word until valid word is selected.
	while (choose_word(answer));
	
	strcpy(word, answer);
	for (i = 0; word[i]; i++) 
		word[i] = '_';
	
	printf("Word selected.\n\n");
		
	printf("%s\n", word);
	
	while (wrong < 6 && strcmp(word, answer)) {
		while (get_guess(answer, word, guesses, &wrong));
		
		show_gallows(guesses, word, wrong);
		printf("\n%s\n", word);
	}
	
	if (wrong > 5) {
		printf("Game over.\n");
		printf("Correct word: %s\n", answer);
	}
	
	if (!strcmp(word, answer))
		printf("Congratulations, you win!\n");
	
	return 0;
}

int get_guess(char* answer, char* word, char* guesses, int *wrong) 
{
	char c[3];
	int i, correct = 0;
	//Get and verify input
	printf("\nGuess: ");
	while (c[1] != '\n') {
		fgets(c, 3, stdin);
	}
	c[1] = 0;
	
	if (!isalpha(c[0])) {
		printf("Enter alphabetic character.");
		return 1;
	}
	
	//Check that input does not match previous guesses
	for (i = 0; guesses[i]; i++) {
		if (c[0] == guesses[i]) {
			printf("%c already guessed.", c[0]);
			return 1;
		}
	}
	
	//Add guess to list, mark end of list with NULL
	for (i = 0; guesses[i]; i++);
	guesses[i] = c[0];
	guesses[i + 1] = '\0';
	
	//Check if guess is correct
	for (i = 0; answer[i]; i++) {
		if (answer[i] == c[0]) {
			word[i] = c[0];
			correct = 1;
		}
	}
	
	//Increment wrong guess count if incorrect
	if (correct) {
		correct = 0;
		return 0;
	}
	else {
		*wrong += 1;
		return 0;
	}
}

void show_gallows(char* guesses, char* word, int wrong)
{
	int i, j, flag = 0;
	printf("\n  +----+   Incorrect\n");
	if (wrong > 0)
		printf("  O    |   ");
	else
		printf("       |   ");
	for (i = 0; guesses[i]; i++) {
		for (j = 0; word[j]; j++) {
			if (guesses[i] == word[j])
				flag = 1;
		}
		if (flag)
			flag = 0;
		else
			printf("%c ", guesses[i]);
	}
	printf("\n");
	if (wrong >= 2) {
		switch (wrong) {
			case 2: printf("  |    |\n"); break;
			case 3: printf(" /|    |\n"); break;
			default: printf(" /|\\   |\n"); break;
		}
	} else
		printf("       |\n");
	
	switch (wrong) {
		case 5: printf(" /     |\n"); break;
		case 6: printf(" / \\   |\n"); break;
		default: printf("       |\n");
	}
	
	printf("_______|_\n");
	
	return;
}

int choose_word(char* answer)
{
	int i, j;
	size_t ln;
	
	srand(time(NULL));
	j = rand() % WORD_POOL;
	
	for (i = 0; i < j; i++) {
		fgets(answer, 50, fpin);
	}
	ln = strlen(answer) - 1;
	if (answer[ln] == '\n' || answer[ln] == '\r') 
		answer[ln] = '\0';
	
	for (i = 0; answer[i]; i++) {
		if (!isalpha(answer[i]))
			return 1;
	}
	
	return 0;
}
