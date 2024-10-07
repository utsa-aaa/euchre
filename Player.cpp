#include <iostream>
#include "Player.hpp"
#include <cassert>
#include <algorithm>
#include <vector>
using namespace std;

class SimplePlayer: public Player{
    private:
    string name;
    vector<Card> player_hand;

    public:
    SimplePlayer(string nameString)
    {
        name = nameString;
    }

    const std::string & get_name() const{
        return name;
    }

    void add_card(const Card &c){
        assert(player_hand.size()<MAX_HAND_SIZE);
        player_hand.push_back(c);
    }
    
    bool make_trump(const Card &upcard, bool is_dealer,
    int round, Suit &order_up_suit) const{
        assert(round==1 || round==2);
        Suit upcardSuit = upcard.get_suit();

        if (is_dealer && round==2) 
        {
            order_up_suit = Suit_next(upcardSuit);
            return true;
        }

        int needed = 2;
        for (int num = 0; num < player_hand.size(); num++)
        {
            if (player_hand[num].is_face_or_ace())
            {
                if (round == 1 && (player_hand[num].get_suit(upcardSuit) == upcardSuit))
                {
                    needed -= 1;
                }
                else if (round == 2 && (player_hand[num].get_suit() 
                    == Suit_next(upcardSuit)))
                {
                    needed -= 1;
                }
            }
        }

        if (round == 1 && needed <= 0)
        {
            order_up_suit = upcardSuit;
            return true;
        }
        else if (round == 2 && needed <= 1)
        {
            order_up_suit = Suit_next(upcardSuit);
            return true;
        }
        return false;
    }
    
    void add_and_discard(const Card &upcard){
        assert(player_hand.size()>=1);
    
        Card lowest = player_hand[0];
        Suit upcardSuit = upcard.get_suit();
        int index = 0;

        for (int num = 1; num < player_hand.size(); num++)
        {
            if (Card_less(player_hand[num], lowest, upcardSuit))
            {
                lowest = player_hand[num];
                index = num;
            }
        }
        if (Card_less(upcard, lowest, upcardSuit))
        {
            return;
        }
        player_hand.erase(player_hand.begin() + index);
        player_hand.push_back(upcard);
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed from the player's hand.
    Card lead_card(Suit trump){
        assert(player_hand.size()>=1);

        Card highest = player_hand[0];
        int index = 0;

        for (int num = 1; num < player_hand.size(); num++)
        {
            if (highest.is_trump(trump) && !player_hand[num].is_trump(trump))
            {
                highest = player_hand[num];
                index = num;
            }
            else if (!highest.is_trump(trump) && player_hand[num].is_trump(trump))
            {
                //Don't do anything
            }
            else if (Card_less(highest, player_hand[num], trump))
            {
                highest = player_hand[num];
                index = num;
            }
        }

        player_hand.erase(player_hand.begin() + index);
        return highest;
    }

    Card play_card(const Card &led_card, Suit trump)
    {
        Suit ledSuit = led_card.get_suit(trump);
        Card choice = player_hand[0];
        int index = 0;

        for (int num = 1; num < player_hand.size(); num++)
        {
            if (player_hand[num].get_suit(trump) == ledSuit)
            {
                if (choice.get_suit(trump) != ledSuit)
                {
                    choice = player_hand[num];
                    index = num;
                }
                else if (Card_less(choice, player_hand[num], trump))
                {
                    choice = player_hand[num];
                    index = num;
                }
            }
            else if(choice.get_suit(trump) != ledSuit &&
                Card_less(player_hand[num], choice, trump))
            {
                choice = player_hand[num];
                index = num;
            }
        }

        player_hand.erase(player_hand.begin() + index);
        return choice;
    }
};

class HumanPlayer : public Player
{
    private:
    string name;
    vector<Card> player_hand;

    public:
    HumanPlayer(string nameString)
    {
        name = nameString;
    }

        const std::string & get_name() const{
        return name;
    }

    void add_card(const Card &c){
        assert(player_hand.size()<MAX_HAND_SIZE);
        player_hand.push_back(c);
        std::sort(player_hand.begin(), player_hand.end());
    }
    
  
    bool make_trump(const Card &upcard, bool is_dealer,
    int round, Suit &order_up_suit) const{
        assert(round==1 || round==2);
        string decision;
        print_hand();
        cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
        cin>>decision;
        if (decision != "pass") {
            order_up_suit = string_to_suit(decision);
            return true;
        }
        return false;
    }
    
    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    void add_and_discard(const Card &upcard){
        assert(player_hand.size()>=1);
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";
        int discard;
        cin>>discard;
        if (discard < 0){

            return;
        }
        player_hand.erase(player_hand.begin()+discard);
        player_hand.push_back(upcard);
        
        std::sort(player_hand.begin(), player_hand.end());
    }

    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed from the player's hand.
    Card lead_card(Suit trump){
        assert(player_hand.size()>=1);
        print_hand();
        int card;
        cout << "Human player " << name << ", please select a card:\n";
        cin>>card;
        Card play = player_hand[card];
        player_hand.erase(player_hand.begin()+card);
        return play;
    }

    Card play_card(const Card &led_card, Suit trump)
    {
        print_hand();
        int card;
        cout << "Human player " << name << ", please select a card:\n";
        cin>>card;
        Card play = player_hand[card];
        player_hand.erase(player_hand.begin()+card);
        return play;
    }

    void print_hand() const {
        for (size_t i=0; i < player_hand.size(); ++i)
        {
            cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << player_hand[i] << "\n";
        }
    }
};

Player * Player_factory(const std::string &name, const std::string &strategy) {
  if (strategy == "Simple") {
    return new SimplePlayer(name);
  }
  else if(strategy == "Human"){
    return new HumanPlayer(name);
  }
  assert(false);
  return nullptr;
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
    os << p.get_name();
    return os;
}