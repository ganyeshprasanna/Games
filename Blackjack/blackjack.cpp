#include <iostream>
#include <array>
#include <ctime> // for time()
#include <cstdlib> // for rand() and srand()
 
class Card
{
public:
	enum CardSuit
	{
		SUIT_CLUB,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_SPADE,
		MAX_SUITS
	};
	 
	enum CardRank
	{
		RANK_2 = 2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_10,
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		MAX_RANKS
	};

private:	
	Card::CardRank m_rank;
	Card::CardSuit m_suit;

public:
	Card(Card::CardRank rank = RANK_2, Card::CardSuit suit = SUIT_SPADE)
		: m_rank(rank), m_suit(suit)
	{

	}

	void printCard() const
	{
		switch (m_rank)
		{	
			case RANK_2:		std::cout << '2'; break;
			case RANK_3:		std::cout << '3'; break;
			case RANK_4:		std::cout << '4'; break;
			case RANK_5:		std::cout << '5'; break;
			case RANK_6:		std::cout << '6'; break;
			case RANK_7:		std::cout << '7'; break;
			case RANK_8:		std::cout << '8'; break;
			case RANK_9:		std::cout << '9'; break;
			case RANK_10:		std::cout << 'T'; break;
			case RANK_JACK:		std::cout << 'J'; break;
			case RANK_QUEEN:	std::cout << 'Q'; break;
			case RANK_KING:		std::cout << 'K'; break;
			case RANK_ACE:		std::cout << 'A'; break;
		}
	 
		switch (m_suit)
		{
			case SUIT_CLUB:		std::cout << 'C'; break;
			case SUIT_DIAMOND:	std::cout << 'D'; break;
			case SUIT_HEART:	std::cout << 'H'; break;
			case SUIT_SPADE:	std::cout << 'S'; break;
		}
	}

	int getCardValue() const
	{
		switch (m_rank)
		{
			case RANK_2:		return 2;
			case RANK_3:		return 3;
			case RANK_4:		return 4;
			case RANK_5:		return 5;
			case RANK_6:		return 6;
			case RANK_7:		return 7;
			case RANK_8:		return 8;
			case RANK_9:		return 9;
			case RANK_10:		return 10;
			case RANK_JACK:		return 10;
			case RANK_QUEEN:	return 10;
			case RANK_KING:		return 10;
			case RANK_ACE:		return 11;
		}
		
		return 0;
	}
}; 

class Deck
{
 std::array<Card,52> m_deck;

public:
	Deck()
	{
		int rank = 0;
		int suit = 0;
		for (Card &card:m_deck)
		{
			card = Card((static_cast<Card::CardRank>((rank%13)+2)),(static_cast<Card::CardSuit>(suit)));
			if ((++rank)%13==0)
				suit++;
		}
	}

	void printDeck() const
	{
		for (const Card &card : m_deck)
		{
			card.printCard();
			std::cout << ' ';
		}
	 
		std::cout << '\n';
	}
 
	void swapCard(Card &a, Card &b)
	{
		Card temp = a;
		a = b;
		b = temp;
	}
	 
	int getRandomNumber(int min, int max)
	{
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}
	 
	Card dealCard() const
	{
		static const Card *ptr = &m_deck[0];
		return *ptr++;
	} 

	void shuffleDeck()
	{
		for (Card &card:m_deck)
		{
			int swapIndex = getRandomNumber(0, 51);
			swapCard(card, m_deck[swapIndex]);
		}
	}	 
};
 
class Blackjack
{
	Deck deck;
	char choice;

public:
	Blackjack()
	{
		deck.shuffleDeck();
	}

	char getPlayerChoice()
	{
		std::cout << "(h) to hit, or (s) to stand: ";
		do
		{
			std::cin >> choice;
		} while (choice != 'h' && choice != 's');
		
		return choice;
	}

	bool playBlackjack()
	{
		int playerTotal = 0;
		int dealerTotal = 0;
		dealerTotal += deck.dealCard().getCardValue();
		std::cout << "The dealer is showing: " << dealerTotal << '\n';
		playerTotal += deck.dealCard().getCardValue();
		playerTotal += deck.dealCard().getCardValue();
		while (1)
		{
			std::cout << "You have: " << playerTotal << '\n';
			getPlayerChoice();
			if (choice == 's')
				break;
	 
			playerTotal += deck.dealCard().getCardValue();
			if (playerTotal > 21)
				return false;
		}

		while (dealerTotal < 17)
		{
			dealerTotal += deck.dealCard().getCardValue();
			std::cout << "The dealer now has: " << dealerTotal << '\n';
		}
		if (dealerTotal > 21)
			return true;
		
		return (playerTotal > dealerTotal);
		}
};

int main()
{
	srand(static_cast<unsigned int>(time(0))); 
	rand();
	Blackjack blackjack;
	bool win = blackjack.playBlackjack();
	if (win)
		std::cout << "You win!\n";
	else
		std::cout << "You lose!\n";
	return 0;
}