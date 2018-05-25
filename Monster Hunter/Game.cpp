/*
A simple game where you fight monsters. The goal of the game is to collect as much gold as you can before
you die or get to level 20.
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

/* Random number generator because even though rand() and srand() are random they produce the same kind 
   of random results when they are run every time. This is because they do simple mathematical operations
   on the seed. */
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

// Creature class
/* Every creature starts with a name, a symbol, a certain amount of health, a certain amount of damage that
   each of their attack deals and a certain amount of gold */

class Creature
{
	std::string m_name;
	char m_symbol;
	int m_health;
	int m_gold;	

protected:
	int m_attackDamage;

public:
	Creature(std::string name = " ", char symbol = 'a', int health = 0, int attackDamage = 0, int gold = 0):
		m_name(name),m_symbol(symbol),m_health(health),m_attackDamage(attackDamage),m_gold(gold)
	{

	}
	// Function adds gold to the existing gold
	void addGold(const int &gold)
	{
		m_gold += gold;
	}

	// When a creature is dealt with damage from  a fight this function reduces its health
	void reduceHealth(const int &damage)
	{
		m_health -= damage;
	}

	// A getter/setter for the creature's name
	std::string& getName() 
	{
		return m_name;
	}

	// A bunch of getters for health, gold, attack damage and symbol
	int getHealth() const
	{
		return m_health;
	}

	int getGold() const
	{
		return m_gold;
	}

	int getAttackDamage() const
	{
		return m_attackDamage;
	}

	char getSymbol() const
	{
		return m_symbol;
	}

	// Checks if creature is dead. A creature is dead when its health drops to zero.
	bool isDead()
	{
		return m_health <= 0;
	}
};

// A player class that inherits from the Creature class. (Player is a Creature)

/*  Player has an additional member, the player's level, which starts at 1. The player has a custom name 
    (entered by the user), uses symbol '@', has 10 health, does 1 damage to start, and has no gold to start with. */

class Player : public Creature
{
	int m_level;

public:
	Player(std::string name = " ", int level = 1):
		Creature(name,'@',10,1,0),m_level(level)
	{

	}

//  This function increases the player's level and damage by 1
	void levelUp()
	{
		++m_level;
		++m_attackDamage;
	}

//  A getter for the level member
	int getLevel()
	{
		return m_level;
	}

//  The player wins if they reach level twenty. This function takes care of that.
	bool hasWon()
	{
		return m_level == 20;
	}
};

// A monster class that inherits from the Creature class. (Monster is a Creature)

/* A monster can be of three types - a dragon, orc or a slime. Each monster type has certain specific values
   of the creature attributes (health, gold, name, attackDamage and symbol). */

class Monster : public Creature
{
public:

/*  An enum to hold the Monster type. Enum was used instead of enum class because C++ supports implicit type 
	from enum to int and also the scope operator is not necessary, thus making the code neat and readable. */

	enum MonsterType
	{
		DRAGON,
		ORC,
		SLIME,
		MAX_TYPES
	};

//  A stuct that will hold the data for each monster type
	struct MonsterData
	{
		std::string name;
		char symbol;
		int health;
		int attackDamage;
		int gold;
	};

private:

/*  An array of structs of type MonsterData that hold the data for the corresponding monster type. This is
	declared static because it is an array of structs. Since there will be a large number of monsters 
	created in the course of the game, for each instance this variable will be created thereby consuming 
	memory. Thus static variable is used. */
	
	static MonsterData monsterData[MAX_TYPES];

public:
	// The default constructor
	Monster():
		Creature()
	{

	}
	
	// Constructor with initializer list
	Monster(MonsterType type):
		Creature(monsterData[type].name,\
			monsterData[type].symbol,\
			monsterData[type].health,\
			monsterData[type].attackDamage,\
			monsterData[type].gold)
	{

	}

// Has to be static as it is used for creating a monster and not associated with any single instance.
	static Monster getRandomMonster()
	{
		return Monster(static_cast<MonsterType>(getRandomNumber(0,MAX_TYPES-1)));
	}
};

/* Static variables are not connected with an instance object and can be accessed even before an 
   instance is created */
Monster::MonsterData Monster::monsterData[Monster::MAX_TYPES]
{
	{ "dragon", 'D', 20, 4, 100 },
	{ "orc", 'o', 4, 2, 25 },
	{ "slime", 's', 1, 1, 10 }
};


/* Game class of which player and monsters are part of. Notice that "is a" relationships are modeled 
   using an inheritance whereas "part of" relationships are handled with Composition (which is the 
   simpler one between composition and aggregation) 

Rules of the game:   

1.  The player encounters one randomly generated monster at a time.
2.  For each monster, the player has two choices: (R)un or (F)ight.
3.  If the player decides to Run, they have a 50% chance of escaping.
4.  If the player escapes, they move to the next encounter with no ill effects.
5.  If the player does not escape, the monster gets a free attack, and the player chooses their next 
   action.
6.  If the player chooses to fight, the player attacks first. The monster's health is reduced by the 
	   player's damage.
7.  If the monster dies, the player takes any gold the monster is carrying. The player also levels up, 
	   increasing their level and damage by 1.
8.  If the monster does not die, the monster attacks the player back. The player's health is reduced 
   by the monster's damage.
9.  The game ends when the player has died (loss) or reached level 20 (win)
10. If the player dies, the game should tell the player what level they were and how much gold they had.
11. If the player wins, the game should tell the player they won, and how much gold they had
   */
class Game
{
	Player player;
	Monster monster;
	
public:
	Game()
	{
		std::cout << "Enter your name: ";
		std::string playerName;
		std::cin >> playerName;		
		player.getName() = playerName;
		std::cout<<player.getHealth();
	}

//	A function that determines whether the player can flee or not when he chooses to	
	bool run()
	{
		return (getRandomNumber(1,100)<=50);
	}

// A function that enables the player to attack the monster
	void attackMonster()
	{
		std::cout<<"You hit the "<<monster.getName()<<" for "<<player.getAttackDamage()<<" damage.\n";
		monster.reduceHealth(player.getAttackDamage());
	}

// A function that enables the monster to attack the player
	void attackPlayer()
	{
		std::cout<<"The "<<monster.getName()<<" hit you for "<<monster.getAttackDamage()<<" damage.\n";
		player.reduceHealth(monster.getAttackDamage());
	}

// A function that checks if the player has been killed and tells the player.
	bool playerKilled()
	{
		if (player.isDead())
		{
			std::cout<<"You died at level "<<player.getLevel()<<" and with "<<player.getGold()<<" gold.\n";
			std::cout<<"Too bad you can't take it with you!\n";
			return true;
		}
		return false;
	}

// A function that checks if the monster has been killed and tells the player.
	bool monsterKilled()
	{
		if (monster.isDead())
		{
			std::cout<<"You killed the "<<monster.getName()<<".\n";
			std::cout<<"You are now level "<<player.getLevel()<<".\n";
			std::cout<<"You found "<<monster.getGold()<<" gold.\n";
			player.levelUp();
			player.addGold(monster.getGold());
			return true;
		}
		return false;
	}

// A function that enables that player to interactively fight with the monster.
	void fightMonster()
	{
		char decision;
		bool fight = true;
			while(fight)
			{
				std::cout<<"(R)un or (F)ight:";
				std::cin>>decision;
				
				if (decision == 'f')
				{
					attackMonster();
					if (monsterKilled())
					{
						return;
					}
					attackPlayer();
					if (playerKilled())
					{
						return;
					}
				}

				else if (decision == 'r')
				{
					fight = !(run());				
					if (fight)
					{	
						std::cout<<"You failed to flee.\n";
						attackPlayer();
						if (playerKilled())
						 {
							return;
						}
					}
					else
						std::cout<<"You sucessfully fled.\n";
				}
			}
	}

// Function to start playing the game
	void play()
	{
		int count = 0;
		std::cout<<"Welcome, "<<player.getName()<<".\n";
		do 
		{
			monster = Monster::getRandomMonster();
			std::cout << "You have encountered a " << monster.getName() << " (" << monster.getSymbol() << ") \n";
			fightMonster();	
		} while(!player.isDead() && !player.hasWon());

		if (player.hasWon())
		{	
			std::cout<<"You have won with "<<player.getGold()<<" gold.\n";
		}
	}
};

int main()
{
	srand(static_cast<unsigned int>(time(0))); 
	rand();
	Game game;
	game.play();
	return 0;
}