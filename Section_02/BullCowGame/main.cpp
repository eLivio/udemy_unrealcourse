/*
This is the main class of the program.
This class is responsible for all interactions with the user.
To see game logic check FBullCowGame class.
*/

#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include "FBullCowGame.h"

//To make syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
FText GetValidGuess();
void PrintGuess(FBullCowCount);
bool AskToPlayAgain();
void PrintGameSummary();

//New game instance, used across all games.
FBullCowGame BCGame;

int main() 
{
	bool bPlayAgain = false;
	do 
	{
		PrintIntro();
		PlayGame();
		bPlayAgain = AskToPlayAgain();
	} while (bPlayAgain);
	
	return 0; // Exit application
}

void PrintIntro()
{	
	std::cout << "Welcome to Bulls and Cows\n" << std::endl;
	std::cout << "\\|/          (__)        __ " << std::endl;
	std::cout << "     `\\------(oo)       (oo)------/`" << std::endl;
	std::cout << "       ||    (__)       (__)    ||" << std::endl;
	std::cout << "       ||v--||     \\|/     ||--w||" << std::endl;
	std::cout << "   \\|/                            \\|/\n" << std::endl;
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength() << " letter isogram I'm thinking of?" << std::endl;
}

//Logic from one game from start to end
void PlayGame() 
{
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	std::cout << "You have " << MaxTries << " tries." << std::endl;

	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries)
	{
		FText Guess = GetValidGuess();

		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		PrintGuess(BullCowCount);
	}

	PrintGameSummary();
	
}

// Ask the user to guess
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";

	do
	{
		std::cout << "\n Try " << BCGame.GetCurrentTry() << ". You have " << (BCGame.GetMaxTries() - BCGame.GetCurrentTry() + 1) << " left.\n Guess the word: ";
		std::getline(std::cin, Guess);
		std::transform(Guess.begin(), Guess.end(), Guess.begin(), ::tolower);

		Status = BCGame.CheckGuessValidity(Guess);
		
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n";
			break;
		case EGuessStatus::Numbers_Not_Allowed:
			std::cout << "Numbers are not allowed.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Word must not contain repeated letters.\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Word must be written all lowercase.\n";
			break;
		default:
			break;
		}
	} while (Status != EGuessStatus::OK);

	return Guess;
}

// Print back the guess
void PrintGuess(FBullCowCount BullCowCount)
{
	std::cout << "Bulls: " << BullCowCount.Bulls << " & Cows: " << BullCowCount.Cows << std::endl;
}

bool AskToPlayAgain()
{
	std::cout << "\nDo you want to play again with the same hidden word? (y/N) ";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y' || Response[0] == 'Y');
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "\nYou WON the game!!\n";
	}
	else
	{
		std::cout << "\nBetter luck next time!\n";
	}

	return;
}