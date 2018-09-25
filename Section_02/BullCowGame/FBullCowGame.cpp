/*
This class contains all the logic of the game.
*/

#pragma once

#include "FBullCowGame.h"
#include <map>

//Make syntax Unreal friendly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() { Reset(); } //Default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTries; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{
	TMap<int32, int32> WordLengthToMaxTries{ {3,4}, {4,7}, {5,10}, {6,16}, {7,20} };
	
	return WordLengthToMaxTries[MyHiddenWord.length()];
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "bolas"; //This MUST be an isogram

	MyHiddenWord = HIDDEN_WORD;
	MyCurrentTries = 1;
	bGameIsWon = false;

	return;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetHiddenWordLength())
	{
		return EGuessStatus::Wrong_Length;
	}
	else if (!std::regex_match(Guess, std::regex("^[a-zA-Z]+$")))
	{
		return EGuessStatus::Numbers_Not_Allowed;
	}
	//else if (std::regex_match(Guess, std::regex("^.*(.).*\\1.*$")))
	else if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	else if (!IsLowercase(Guess))
	{
		return EGuessStatus::Not_Lowercase;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter);

		if (LetterSeen[Letter])
		{
			return false;
		}
		else
		{
			LetterSeen[Letter] = true;
		}
	}

	return true;
}

bool FBullCowGame::IsLowercase(FString Word) const
{
	for (auto Letter : Word)
	{
		if (!islower(Letter))
		{
			return false;
		}
	}

	return true;
}

FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTries++;

	FBullCowCount BullCowCount;

	int32 WordLength = MyHiddenWord.length();
	for (int32 MHWChat = 0; MHWChat < WordLength; MHWChat++) 
	{
		for (int32 GChar = 0; GChar < WordLength; GChar++) 
		{
			if (MyHiddenWord[MHWChat] == Guess[GChar]) 
			{
				if (MHWChat == GChar)
					BullCowCount.Bulls++;
				else
					BullCowCount.Cows++;
			}
		}
	}

	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}

	return BullCowCount;
}
