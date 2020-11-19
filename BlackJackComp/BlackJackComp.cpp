#include <iostream>
#include <ctime>
#include <vector>

char ToUpperCase(char c);

int GetRandomInt(int max);
int DrawCard(bool player, std::vector<int> cardSum);
int SumOfCards(std::vector<int> cards);
int PlayRound(int& currentBet, int& playerMoney, int& houseMoney);
int AskBetSum(int, int, int);

void DrawCardFee(int& playerMoney, int& houseMoney);
void PrintTitle(int playerMoney, int houseMoney);
void DrawInitialCards(std::vector<int>& cards, bool isPlayer);

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	const int STARTER_MONEY = 100;
	const int PLAY_FEE = 10;
	int currentBet = 0;
	int playerMoney = STARTER_MONEY;
	int houseMoney = STARTER_MONEY;
	// win = 1, draw = 0, lose = -1
	int currentResult = 0;
	char playerChoice = ' ';

	bool invalidChoice = false;
	bool playAgain = false;

	do
	{
		// Prints title to console
		PrintTitle(playerMoney, houseMoney);

		// Loops until either player or house is out of money
		do
		{
			// PlayRound returns: -1 if lost, 0 if draw and 1 if won
			currentResult = PlayRound(currentBet, playerMoney, houseMoney);
			if (currentResult == 1)
				playerMoney = playerMoney + (currentResult * currentBet) + 10; // Gives player back draw fee + prize
			else
				playerMoney = playerMoney + (currentResult * currentBet); // Negative if lost
			if (currentResult == 1)
				houseMoney = houseMoney - (currentResult * currentBet) - 10; // Takes draw fee from house + prize
			else
				houseMoney = houseMoney - (currentResult * currentBet); // Negative if lost

			// Only for when player quits mid round
			if (playerMoney < 0)
				playerMoney = 0;
			std::cout << "You got " << playerMoney << "$" << std::endl;
			// Only for when player quits mid round
			if (houseMoney > 200)
				houseMoney = 200;
			std::cout << "The house got " << houseMoney << "$" << std::endl << std::endl;
		} while (playerMoney > 0 && houseMoney > 0);

		if (playerMoney <= 0)
		{
			std::cout << "You are out of money! Game over!" << std::endl;
		}
		if (houseMoney <= 0)
		{
			std::cout << "The house are out of money! You win!" << std::endl;
		}

		do
		{
			std::cout << "Do you want to play again? y/n" << std::endl;
			std::cin >> playerChoice;
			switch (ToUpperCase(playerChoice))
			{
			case 'Y':
				playAgain = true;
				invalidChoice = false;
				playerMoney = STARTER_MONEY;
				houseMoney = STARTER_MONEY;
				break;
			case 'N':
				playAgain = false;
				invalidChoice = false;
				break;
			default:
				std::cout << "Invalid choice! Try again..." << std::endl;
				invalidChoice = true;
				break;
			}
		} while (invalidChoice);
		std::cout << std::endl;
	} while (playAgain);
}

// Calculate the sum of your cards
int SumOfCards(std::vector<int> cards)
{
	int sum = 0;
	for (int i = 0; i < cards.size(); i++)
	{
		sum += cards.at(i);
	}
	return sum;
}

// Draws a card to the player if the player variable is true
// Draws a card to the house if the player variable is false
int DrawCard(bool player, std::vector<int> cards)
{
	int number = GetRandomInt(10);
	bool invalidChoice = false;
	// if player gets an Ace ask him/her if it should be 1 or 11
	if (number == 1 && player)
	{
		std::cout << "You drew an ace! Your current sum = " << SumOfCards(cards) << std::endl << "Choose between 1 or 11: ";
		do
		{
			std::cin >> number;
			switch (number)
			{
			case 1:
				number = 1;
				invalidChoice = false;
				break;
			case 11:
				number = 11;
				invalidChoice = false;
				break;
			default:
				invalidChoice = true;
				std::cout << "Invalid choice, choose between 1 and 11." << std::endl;
				break;
			}
		} while (invalidChoice);
	}
	// Do AI logic if the AI gets and Ace
	else if (number == 1 && !player)
	{
		std::cout << "The house drew an ace!" << std::endl;
		// AI chooses 1 if he already have 6 or more than 6
		if (SumOfCards(cards) <= 6)
		{
			number = 1;
		}
		// Also chooses 1 if sum of cards is greater than 10
		else if (SumOfCards(cards) > 10)
		{
			number = 1;
		}
		else
		{
			number = 11;
		}
		std::cout << "It chose " << number << std::endl;
	}

	// Don't print it again if Ace
	if (player && (number != 1 && number != 11))
	{
		std::cout << "You drew a " << number << std::endl;
	}
	else if (!player && (number != 1 && number != 11))
	{
		std::cout << "The house drew a " << number << std::endl;
	}
	return number;
}

// Asks the player how much he wants to bet
int AskBetSum(int currentBet, int playerMoney, int houseMoney)
{
	bool invalidChoice = false;
	do
	{
		std::cout << "Choose amount to bet: ";
		std::cin >> currentBet;
		if (currentBet <= 0)
		{
			std::cout << "The amount you bet needs to be more than 0." << std::endl;
			invalidChoice = true;
		}
		else if (currentBet > playerMoney)
		{
			std::cout << "The amount you bet needs to be equal or less than your money." << std::endl;
			invalidChoice = true;
		}
		else if (currentBet > houseMoney)
		{
			std::cout << "The amount you bet needs to be equal or less than the houses money." << std::endl;
			invalidChoice = true;
		}
		else
		{
			invalidChoice = false;
		}
		std::cin.clear();
		std::cin.ignore(256, '\n');
	} while (invalidChoice);
	std::cout << std::endl;
	return currentBet;
}

// Draws the two first cards of a round
void DrawInitialCards(std::vector<int>& cards, bool isPlayer)
{
	cards.push_back(DrawCard(isPlayer, cards));
	cards.push_back(DrawCard(isPlayer, cards));
	if (isPlayer)
		std::cout << "Your current card sum is " << SumOfCards(cards) << std::endl << std::endl;
	else
		std::cout << "The houses cards sum up to " << SumOfCards(cards) << std::endl << std::endl;
}

// Takes the draw card fee from player and gives it to the house
void DrawCardFee(int& playerMoney, int& houseMoney)
{
	playerMoney -= 10;
	houseMoney += 10;
}

int PlayRound(int& currentBet, int& playerMoney, int& houseMoney)
{
	char playerInput = ' ';

	int winLoseDraw = 0; // 1 = win, -1 = lose, 0 = draw
	bool staying = false;
	bool playerTurn = false;
	bool playing = false;
	bool invalidChoice = false;
	std::vector<int> playerCards;
	std::vector<int> houseCards;

	std::cout << "We take a 10$ fee to give out cards.\nYou will get them back if you win." << std::endl;
	DrawCardFee(playerMoney, houseMoney);
	DrawInitialCards(playerCards, true);
	DrawInitialCards(houseCards, false);


	std::cout << "C. Continue to betting." << std::endl;
	std::cout << "F. Fold your hand." << std::endl;
	std::cout << "Q. Quit game." << std::endl;
	do
	{
		std::cout << "Choose one of the options: ";
		std::cin >> playerInput;
		switch (ToUpperCase(playerInput))
		{
		case 'C':
			playing = true;
			invalidChoice = false;
			break;
		case 'F':
			playing = false;
			invalidChoice = false;
			break;
		case 'Q':
			invalidChoice = false;
			return -100;
			break;
		default:
			std::cout << "Invalid input! Try again..." << std::endl;
			invalidChoice = true;
			break;
		}
	} while (invalidChoice);

	if (playing)
	{
		currentBet = AskBetSum(currentBet, playerMoney, houseMoney);
	}
	
	while (SumOfCards(playerCards) < 21 && !staying && playing)
	{
		playerTurn = true;
		std::cout << "D. Draw a card." << std::endl;
		std::cout << "S. Stay with your cards." << std::endl;
		std::cout << "F. Fold your cards." << std::endl;
		std::cout << "Please choose one of the options: ";
		std::cin >> playerInput;
		std::cout << std::endl;

		switch (ToUpperCase(playerInput))
		{
		case 'D':
			playerCards.push_back(DrawCard(playerTurn, playerCards));
			break;
		case 'S':
			staying = true;
			break;
		case 'F':
			playerCards.erase(playerCards.begin(), playerCards.end());
			staying = true;
			break;
		default:
			break;
		}
		std::cout << "Current sum: " << SumOfCards(playerCards) << std::endl << std::endl;
	};

	if (SumOfCards(playerCards) == 21)
	{
		winLoseDraw = 1;
		std::cout << "You got 21, you win!" << std::endl;
	}
	else if (SumOfCards(playerCards) > 21)
	{
		winLoseDraw = -1;
		std::cout << "You got more than 21, you lose!" << std::endl;
	}

	playerTurn = false;
	if (winLoseDraw == 0 && playing)
	{
		std::cout << "House is drawing..." << std::endl;
		do
		{
			if (SumOfCards(houseCards) < SumOfCards(playerCards))
			{
				houseCards.push_back(DrawCard(playerTurn, houseCards));
			}
			else if (SumOfCards(houseCards) == SumOfCards(playerCards) && SumOfCards(houseCards) <= 16)
			{
				houseCards.push_back(DrawCard(playerTurn, houseCards));
			}
			std::cout << "House current sum: " << SumOfCards(houseCards) << std::endl;
		} while (SumOfCards(houseCards) < SumOfCards(playerCards) && !(SumOfCards(houseCards) > 21));
		std::cout << std::endl;
	}

	if ((SumOfCards(playerCards) > SumOfCards(houseCards) && SumOfCards(playerCards) <= 21) && winLoseDraw == 0 && playing)
	{
		winLoseDraw = 1;
		std::cout << "You win!" << std::endl << std::endl;
	}
	else if (SumOfCards(houseCards) > 21 && playing)
	{
		winLoseDraw = 1;
		std::cout << "House got more than 21! You win!" << std::endl << std::endl;
	}
	else if ((SumOfCards(houseCards) > SumOfCards(playerCards)) && SumOfCards(houseCards) <= 21 && playing)
	{
		winLoseDraw = -1;
		std::cout << "You lose!" << std::endl << std::endl;
	}

	return winLoseDraw;
}

void PrintTitle(int playerMoney, int houseMoney)
{
	std::cout << "============================" << std::endl;
	std::cout << "|| Welcome to Black Jack! ||" << std::endl;
	std::cout << "============================" << std::endl << std::endl;
	std::cout << "You got " << playerMoney << "$." << std::endl;
	std::cout << "The house got " << houseMoney << "$." << std::endl << std::endl;
}

int GetRandomInt(int max)
{
	int randomNumber = rand() % max + 1;
	return randomNumber;
}

char ToUpperCase(char c)
{
	return toupper(c);
}
