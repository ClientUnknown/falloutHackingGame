// Idea provided by www.reddit.com/r/dailyprogrammer
//
// This program recreates the hacking minigame from Fallout 3 and Fallout New Vegas.
// A randomized word bank is created based on the difficulty choice of the player, a random
// word is chosen to be the key, and the player must make guesses to figure out which word
// is the key.
// The word list is provided by www.reddit.com/r/dailyprogrammer and is a .txt file

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#define MAX_ATTEMPTS 4		// Maximum amount of times the player can guess the word

std::vector<std::string> wordList;

// Function protoypes
void setupGame ();
void getWords (const int, const int);
void displayWordBank ();
void startGame();
void playAgain();

int main () {
	setupGame();
	displayWordBank();
	startGame();

	return 0;
}

// Prepares the word bank by getting the difficulty from the user
// Higher difficulty means more words and longer words
void setupGame () {
	int diffChoice = 0;
	int diffLength = 0;
	do {
		std::cout << "Choose your difficulty (1-5): "; std::cin >> diffChoice;
	} while (diffChoice < 1 || diffChoice > 5);

	switch (diffChoice) {
	case 1: // Very easy
		std::cout << "You chose very easy.\n";
		diffLength = 4;
		wordList.resize(5);
		break;
	case 2: // Easy
		std::cout << "You chose easy.\n";
		diffLength = 6;
		wordList.resize(10);
		break;
	case 3: // Average
		std::cout << "You chose average.\n";
		diffLength = 8;
		wordList.resize(10);
		break;
	case 4: // Hard
		std::cout << "You chose hard.\n";
		diffLength = 12;
		wordList.resize(15);
		break;
	case 5: // Very hard
		std::cout << "You chose very hard.\n";
		diffLength = 15;
		wordList.resize(15);
		break;
	default:
		break;
	}

	getWords(diffChoice, diffLength);
}

// Reads from a database of words, attempting to choose random words
// to insert into the word bank
void getWords (const int diffChoice, const int diffLength) {
	std::ifstream wordDatabase;
	std::string word = " ";
	int startingChar = 0;

	wordDatabase.open("words.txt");

	srand(time(NULL));
	
	// Loop through the vector and fill it with random words with length matching the 
	// length decided by the difficulty choice
	for (int i = 0; i < wordList.size(); i++) {
		do {
			// Read in a random number of lines to help randomize the word bank
			// This is probably a really dumb and horrible way to do this
			for (int k = (rand() % 250); k > 0; k--) {
				wordDatabase >> word;
			}
			wordDatabase >> word;
		} while (word.length() != diffLength);
		wordList[i] = word;
	}
	wordDatabase.close();

	// Shuffle the vector to further randomize the word bank
	std::random_shuffle(wordList.begin(), wordList.end());
}

// Displays the word bank with all words in upper-case
void displayWordBank () {
	for (int i = 0; i < wordList.size(); i++) {
		std::string str = wordList[i];
		// Nested loop to display the words as all upper-case
		for (int k = 0; k < wordList[i].length(); k++) {
			char temp = str[k]-32;
			std::cout << temp;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// Begins the game, asking the player to guess and informing them if their guess is incorrect or correct
// When incorrect, tell the player how close their guess was
void startGame () {
	int attempts = MAX_ATTEMPTS;
	std::string playerGuess = " ";
	std::string password = " ";

	// Choose a word in the bank at random to become the password
	password = wordList[(rand() % wordList.size())];

	while (attempts > 0) {
		int count = 0;
		
		// Check the validity of the user's guess (oh nefarious user)
		do {
			std::cout << "Guess the word (" << attempts << " left): ";
			std::cin >> playerGuess;
			if(playerGuess.length() != password.length())
				std::cout << "Word Length mismatched. Word length should be " << password.length() << ". Please re-enter." << std::endl;
		} while (playerGuess.length() != password.length());

		// Ensure the player's guess is lower case
		for (int i = 0; i < playerGuess.length(); i++) {
			tolower(playerGuess[i]);
		}

		if (playerGuess != password) { // If player's guess isn't correct then check which letters (if any) match
			--attempts;
			for (int i = 0; i < playerGuess.length(); i++) { // See if each letter matches
				if (playerGuess[i] == password[i])
					count++;
			}
			std::cout << count << "/" << playerGuess.length() << " letters correct\n";
		} else { // If player's guess is correct then tell them so and quit the function
			std::cout << playerGuess.length() << "/" << playerGuess.length() << " letters correct\n";
			std::cout << "You win! :D\n";
			playAgain();
			return;
		}
	}
	std::cout << "You lost :( the correct password was: " << password << std::endl;
	playAgain();
}

//Gives option to start a new game after the end of current game session
void playAgain() {
	char playAgainOption;
	std::cout << "Want to play again? (Y/N)" << std::endl;
	std::cin >> playAgainOption;
	if(playAgainOption == 'y' || playAgainOption == 'Y')
		main();
	else {
		std::cout << "Bye" << std::endl;
		return;
	}
}
