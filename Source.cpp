#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <array>
#include <string>
#include <random>
#include <ctime>
#include <cassert>
#include <stdlib.h>

constexpr int blackjacklimit{ 21 };
enum class CardRank
{
Ace,
rank_2, 
rank_3,
rank_4,
rank_5,
rank_6,
rank_7,
rank_8,
rank_9,
rank_10,
Jack, 
Queen, 
King,

max_rank,
};

enum class CardSuit
{
	clubs,
	diamonds,
	hearts, 
	spades,

	max_suit,
};

struct Card
{
	CardSuit suit{};
	CardRank rank{};
};

using Deck = std::array<Card, 52>;
using Index = Deck::size_type;

void printCard(const Card& card)
{

	switch (card.rank)
	{
	case CardRank::Ace:
		std::cout << 'A';
		break;
	case CardRank::rank_2:
		std::cout << '2';
		break;
	case CardRank::rank_3:
		std::cout << '3';
		break;
	case CardRank::rank_4:
		std::cout << '4';
		break;
	case CardRank::rank_5:
		std::cout << '5';
		break;
	case CardRank::rank_6:
		std::cout << '6';
		break;
	case CardRank::rank_7:
		std::cout << '7';
		break;
	case CardRank::rank_8:
		std::cout << '8';
		break;
	case CardRank::rank_9:
		std::cout << '9';
		break;
	case CardRank::rank_10:
		std::cout << 'T';
		break;
	case CardRank::Jack:
		std::cout << 'J';
		break;
	case CardRank::Queen:
		std::cout << 'Q';
		break;
	case CardRank::King:
		std::cout << 'K';
		break;
	}

	switch (card.suit)
	{
	case CardSuit::clubs:
		std::cout << 'C';
		break;
	case CardSuit::diamonds:
		std::cout << 'D'; 
		break;
	case CardSuit::hearts:
		std::cout << 'H';
		break;
	case CardSuit::spades:
		std::cout << 'S';
		break;
	}
}

Deck createDeck()
{
	Index i{0};
	Deck deck;
	for (int suit{ 0 }; suit < 4; suit++) 
	{
		for (int rank{ 0 }; rank < static_cast<int>(CardRank::max_rank); rank++)
		{
			deck[i].suit = static_cast<CardSuit>(suit);
			deck[i].rank = static_cast<CardRank>(rank);
			i++;
		}
	}
	return deck;
 }

void printDeck(const Deck& deck)
{
	for (int i = 0; i < deck.size(); i++)
	{
		printCard(deck[i]);
		std::cout << " ";
		
		if ((i+1) % 11 == 0)
			std::cout << '\n';
	}
	std::cout << '\n';
	
}

Deck shuffleDeck(Deck& deck)
{
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(time(nullptr)) };
	std::shuffle(deck.begin(), deck.end(), mt);
	return deck;
}

int getCardValue(const Deck& deck,int i)
{
	
	
	switch (deck[i].rank)
	{
	case CardRank::Ace:
		return 11;
	case CardRank::rank_2:
		return 2;
	case CardRank::rank_3:
		return 3;
	case CardRank::rank_4:
		return 4;
	case CardRank::rank_5:
		return 5;
	case CardRank::rank_6:
		return 6;
	case CardRank::rank_7:
		return 7;
	case CardRank::rank_8:
		return 8;
	case CardRank::rank_9:
		return 9;
	case CardRank::rank_10:
	case CardRank::Jack:	
	case CardRank::Queen:	
	case CardRank::King:
		return 10;
	default:
		assert(false && "should never happen");
		return 0;
	}
	
}

struct Sums
{
	int dealer{0};
	int player{ 0 };
	int d_aces{ 0 };
	int p_aces{ 0 };
};
int randomCard(const Deck& deck,Sums& sum,char d)
{
	static std::mt19937 mt{ static_cast<std::mt19937::result_type>(time(nullptr)) };
	std::uniform_int_distribution<> distr(0, 51);
	int card{ getCardValue(deck, distr(mt)) };
	if (card == 11)
	{
		if (d == 'p')
			sum.p_aces += 1;
		else
			sum.d_aces += 1;
			
	}
	else
		return card;
}


bool playerControl()
{
	std::cout << "Do you want another card?\nHit(y) or Stand(n)";
	char x{};
	std::cin >> x;
	while (x != static_cast<char> ('y') && x != 'n')
	{
		std::cout << "Invalid input. Please retry:";
		std::cin >> x;
	}
	if (x == static_cast<char> ('y'))
		return true;
	else
		return false;
}

void endgame(int player,int dealer,int daces,int paces)
{
	bool loop = true;
	while (loop == true)
	{
		if (dealer > 21 && daces > 0)

		{
			dealer -= 10;
			daces -= 1;
			std::cout << "Dealer Ace reduced to 1\n";
		}
		if (dealer <= 21 || daces == 0)
		{
			loop = false;
		}
			
	}
	loop = true;
	while (loop == true)
	{
		if (player > 21 && paces > 0)

		{
			player -= 10;
			paces -= 1;
			std::cout << "Player Ace reduced to 1\n";
		}
		if (player <= 21  || paces == 0)
		{
			loop = false;
		}

	}

	if (player > blackjacklimit)
	{
		std::cout << "\tPlayer Lost\n\tPlayer Busted";
	}
	else if (dealer>blackjacklimit)
	{
		std::cout << "\tPlayer Won\n\tDealer Busted";
	}
	else if (player>dealer)
	{
		std::cout << "\tPlayer Won\n\tPlayer: "<<player<<"  >  Dealer: " << dealer;
	}
	else if (dealer>player)
	{
		std::cout << "\tPlayer Lost\n\tPlayer: " << player << "  <  Dealer: " << dealer;
	}
	else if (dealer==player)
	{
		std::cout << "\tDraw\n\tPlayer: " << player << "  =  Dealer: " << dealer;
	}
}

void playBlackjack(const Deck& deck)
{
	Sums sum;
	int dcard{ randomCard(deck,sum,'d') };
	if (dcard == 11)
		++sum.d_aces;

	sum.dealer += dcard;
	

	std::cout << "Dealer gets a card wit sum: " << sum.dealer << std::endl;					//dealer gets one card


	int pcard{ randomCard(deck, sum,'p') };
	if (pcard == 11)
		++sum.p_aces;
	sum.player += pcard;
	std::cout << "Player gets two cards. Player has drawn " << pcard << " and ";
	 pcard= randomCard(deck, sum,'p') ;
	if (pcard == 11)
		++sum.p_aces;
	sum.player += pcard;

	std::cout << pcard<<" with sum: " << sum.player << '\n\n';		// player gets two cards
	bool end = false;

	while(end==false) //players play
	{
		if (playerControl() == false)\
		{
			
			std::cout << "Player stand." << std::endl;
			end = true;
		}
		else
		{
			 pcard= randomCard(deck, sum,'p') ;
			if (pcard == 11)
				++sum.p_aces;
			sum.player += pcard;
			std::cout << "Player has drawn a "<<pcard<<" with a sum: " << sum.player << '\n';
			if (sum.player > blackjacklimit)
			{
				if (sum.p_aces > 0)
				{
						
					--sum.p_aces;
					sum.player -= 10;
					std::cout << "Player Ace reduced to 1 to save BUST. Current sum: "<<sum.player<<'\n';
				}
				else
				{
					std::cout << "\tBUST, player has a score of " << sum.player << '\n';
					end = true;
				}
				
			}
		}	
	}
	bool dend{ false };
	while (dend == false)
	{
		 dcard= randomCard(deck,sum,'d') ;
		if (dcard == 11)
			++sum.d_aces;

		sum.dealer += dcard;
		std::cout << "Dealer has drawn "<<dcard <<" and has a sum of: " << sum.dealer << '\n';

		if (sum.dealer > 17)
		{
			if (sum.dealer > blackjacklimit)
			{
				if (sum.d_aces > 0)
				{
					
					--sum.d_aces;
					sum.dealer -= 10;
					std::cout << "Dealer Ace reduced to 1 to save BUST.Current sum: " << sum.dealer << '\n';
				}
				else
				{
					std::cout << "\tDealer bust: " << sum.dealer << '\n';
					dend = true;
				}
				
			}
			else
			{
				std::cout << "Dealer stand: " << sum.dealer << '\n';
				dend = true;
			}
		}

	}
	std::cout << sum.d_aces << sum.p_aces;
	endgame(sum.player, sum.dealer,sum.d_aces, sum.p_aces);

}

void startgame(Deck& deck)
{
	char x{};
	std::cout << "Would you like to play Blackjack today?\n\t\ty/n";
	std::cin >> x;
	std::cin.clear();
	while (x != static_cast<char> ('y') && x != 'n')
	{
		std::cout << "Invalid input. Please retry:";
		std::cin >> x;
	}
	if (x == static_cast<char> ('y'))
	{
		std::cout << "Game Started\n"<< std::flush;
		system("CLS");
		std::cout << "Blackjack by Edis\n\n\n" << std::flush;
		playBlackjack(deck);
	}
	else
	{
		std::cout << "Game Aborted\n" << std::flush;;
		system("CLS");
	}
		
}
int main()
{
	auto deck{ createDeck() };

	std::cout << "Welcome to Blackjack, heres todays shuffle:";

	shuffleDeck(deck);
	std::cout << std::endl;
	printDeck(deck);
	std::cout << std::endl;
	startgame(deck);
	
	

}