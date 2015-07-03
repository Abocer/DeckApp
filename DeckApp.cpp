
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

typedef enum{spade=0,heart,diamond,club} Suit;	
const string suits[4]={"Spade","Heart","Diamond","Club"};
const string ranks[13]={"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

struct Card
{
	int rank;
	Suit suit;
	void printCard() const
	{
		if(rank>=1 && rank<=13)
			cout<<"("<<suits[(int)suit]<<" "<<ranks[rank-1]<<"), ";
		else	
			cout<<"(invalid card),";
	}
};

/*=====================================================================================*/
class Deck
{
public:	
	virtual void runMain()
	{		
		init();
		while(1)
		{
			cout<<"\n***************************************************************************************************\n";
			cout<<"Please input your option with Deck application:\n"; //match with optionSet in init()						
			cout<<"(1)Draw a card;		(2)Find a card;		(3)Random shuffle;	(4)Three-way-cut shuffle;\n";
			cout<<"(5)Riffle Shuffle;	(6)Reset the deck;	(7)Print the deck;	(10)Quit application;\n";			
			
			string s;
			int input;
			while(1)
			{				
				getline(cin,s);
				if(optionSet.count(s))
				{
					input=atoi(s.c_str());
					break;
				}else
					cout<<"invalid option, please input again\n";
			}
				
			switch(input)
			{
				case 1: 	drawOneCard();		break;
				case 2: 	findOneCard();		break;
				case 3: 	randomShuffle();	break;
				case 4: 	threeWayCut();		break;
				case 5: 	riffleShuffle();	break;
				case 6: 	resetDeck();		break;
				case 7: 	printDeck();		break;
				case 10:
				default:	
					cout<<"Quit application...\n";	
					system("PAUSE");
					return;
			}
		}
	}
/*=====================================================================================*/	
protected:
	static const int numOfCards=52;
	vector<Card> cards;
	unordered_set<string> optionSet;
	unordered_set<string> suitSet;
	unordered_set<string> rankSet;
/*=====================================================================================*/
	static int randomNum(int a1,int a2)  //generate a random integer between a1 and a2
	{
		if(a1==a2)	return a1;
		if(a1>a2)	swap(a1,a2);
		srand((unsigned)time(0));		
		return rand()%(a2-a1+1)+a1;
	}
/*=====================================================================================*/
	virtual void resetDeck()
	{
		cards.clear();
		cards=vector<Card>(numOfCards);
		for(int i=0;i<numOfCards;++i)
		{
			cards[i].rank=i%13+1;
			cards[i].suit=(Suit)(i/13);
		}
		randomShuffle();
		cout<<"Deck reset done\n";
	}
/*=====================================================================================*/
	virtual void init() 
	{
		resetDeck();		
		vector<string> tmp={"1", "2", "3", "4", "5", "6", "7", "10"};		
		optionSet=unordered_set<string>(tmp.begin(), tmp.end());	
		
		tmp={"1", "2", "3", "4"};
		suitSet=unordered_set<string>(tmp.begin(), tmp.end());		
		
		tmp={"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
		rankSet=unordered_set<string>(tmp.begin(), tmp.end());
		cout<<"Deck application initialization completed.\n"; 
	}	
/*=====================================================================================*/	
	Card drawOneCard()
	{
		int sz=cards.size();
		Card ret;
		if(sz<=0)
		{
			cout<<"The deck is empty."<<endl;
			ret.rank=1<<31;
			return ret;
		}
		int idx=randomNum(0,sz-1);
		ret=cards[idx];
		cards.erase(cards.begin()+idx);
		cout<<"Drawing a card from deck done: "; 
		ret.printCard();
		return ret;
	}
/*=====================================================================================*/	
	int findOneCard() const
	{
		Card c;
		string s;
		cout<<"Please input the suit of card to look for:\n";
		cout<<"Spade=1, Heart=2, Diamond=3, Club=4\n"; //match with suitSet in init()
		while(1)
		{
			getline(cin,s);
			if(suitSet.count(s))		
			{
				c.suit=(Suit)(atoi(s.c_str())-1);
				break;
			}
			cout<<"invalid suit, please input again\n";
		}
		
		cout<<"Please input the rank of card to look for (1-13):\n"; //match with rankSet in init()		
		while(1)
		{
			getline(cin,s);
			if(rankSet.count(s))
			{
				c.rank=atoi(s.c_str());
				break;
			}			
			cout<<"invalid rank, please input again\n";
		}		
		
		c.printCard();
		for(unsigned i=0;i<cards.size();++i)
			if(cards[i].rank==c.rank && cards[i].suit==c.suit)
			{
				cout<<" is found in position: "<<i+1<<endl;
				return i+1;
			}
		cout<<" is not found in current deck."<<endl;
		return -1;
	}
/*=====================================================================================*/	
	/*	algorithm principle: pick a random card from n cards and put it in cards[0]; then pick another random card from left n-1 cards and put it in cards[1]; and so on until no card left.	*/
	void randomShuffle() 
	{
		int sz=cards.size();
		for(int i=0;i<sz;++i)		
			swap(cards[i], cards[randomNum(i,sz-1)]);		
		cout<<"Random shuffle done.\n";
	}
/*=====================================================================================*/	
	void threeWayCut()
	{
		int sz=cards.size();
		if(sz<3)
		{
			cout<<"Less than 3 cards in deck, do a random shuffle instead of three-way-cut shuffle\n";
			randomShuffle();
			return;
		}
		int cut1=randomNum(0,sz-3);		//cut1 range: 0 to sz-3
		int cut2=randomNum(cut1+1,sz-2);	//cut2 range: cut1+1 to sz-2
		pair<int,int> piles[3];
		piles[0]=pair<int,int> (0,cut1);
		piles[1]=pair<int,int> (cut1+1,cut2);
		piles[2]=pair<int,int> (cut2+1,sz-1);
		for(int i=0;i<3;++i)	//re-order the three piles randomly
			swap(piles[i], piles[randomNum(i,2)]);
		vector<Card> newCards(sz);
		for(int i=0,pos=0;i<3;++i)		
			for(int j=piles[i].first;j<=piles[i].second;++j)
				newCards[pos++]=cards[j];
		cards=newCards;		
		cout<<"Three-way-cut shuffle done.\n";
	}
/*=====================================================================================*/
	void riffleShuffle()
	{
		int sz=cards.size();
		if(sz<=1)	return;
		//divide deck into two piles with sz1 range between 7*sz/16 and 9*sz/16
		int sz1=randomNum((sz>>1)-(sz>>4),(sz>>1)+(sz>>4)), sz2=sz-sz1;
		if(sz1<=0 || sz2<=0)	return;
		int p=randomNum(1,2), i1=0, i2=0, pos=0;
		vector<Card> newCards(sz);
		while(i1<sz1 && i2<sz2)
		{			
			if(p==1)
			{
				p=2;
				int num=(sz1-i1>=2)?randomNum(1,2):1;  //add 1 or 2 cards to new deck every time
				for(int j=0;j<num;++j)				
					newCards[pos++]=cards[i1++];
				
			}else if(p==2)
			{
				p=1;
				int num=(sz2-i2>=2)?randomNum(1,2):1;  
				for(int j=0;j<num;++j)				
					newCards[pos++]=cards[sz1+(i2++)];
			}
		}
		while(i1<sz1)
			newCards[pos++]=cards[i1++];
		while(i2<sz2)
			newCards[pos++]=cards[sz1+(i2++)];
		if(pos!=sz)	cout<<pos<<"ERROR\n";
		cards=newCards;		
		cout<<"Riffle shuffle done.\n";
	}
/*=====================================================================================*/	
	void printDeck() const
	{
		cout<<"Print cards of current deck:\n";
		for(unsigned i=0;i<cards.size();++i)
			cards[i].printCard();
		cout<<endl;
	}
/*=====================================================================================*/
};


int main()
{
	Deck deck;
	deck.runMain();
	return 0;
}


