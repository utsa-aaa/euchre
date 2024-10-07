#include "Pack.hpp"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

  // EFFECTS: Initializes the Pack to be in the following standard order:
  //          the cards of the lowest suit arranged from lowest rank to
  //          highest rank, followed by the cards of the next lowest suit
  //          in order from lowest to highest rank, and so on. 
  // NOTE: The standard order is the same as that in pack.in.
  // NOTE: Do NOT use pack.in in your implementation of this function
  // NOTE: The pack is initially full, with no cards dealt.
Pack::Pack(){
    int count = 0;
    for(int s=SPADES;s<=DIAMONDS;s++){
        for(int r=NINE;r<=ACE;r++){
            Rank rank = Rank(r);
            Suit suit = Suit(s);
            cards[count]=Card(rank, suit);
            count++;
        }
    }
    reset();
}

// REQUIRES: pack_input contains a representation of a Pack in the
  //           format required by the project specification
  // MODIFIES: pack_input
  // EFFECTS: Initializes Pack by reading from pack_input.
  // NOTE: The pack is initially full, with no cards dealt.
  Pack::Pack(istream& pack_input){
    string ignore;
    string rank;
    string suit;
    int count = 0;
    while(pack_input>>rank>>ignore>>suit){
        cards[count]=Card(string_to_rank(rank), string_to_suit(suit));
        count++;
    }
    reset();
  }
  
  // REQUIRES: cards remain in the Pack
  // EFFECTS: Returns the next card in the pack and increments the next index
  Card Pack:: deal_one(){
    Card drawn = cards[next];
    next++;

    return drawn;
  }

  // EFFECTS: Resets next index to first card in the Pack
  void Pack:: reset(){
     next = 0;
  }


  // EFFECTS: Shuffles the Pack and resets the next index. This
  //          performs an in shuffle seven times. See
  //          https://en.wikipedia.org/wiki/In_shuffle.
  void Pack:: shuffle(){
    for (int num = 0; num < 7; num++)
    {
      int half = floor(cards.size()/2);
      std::array<Card, PACK_SIZE> bottom_half;
      std::array<Card, PACK_SIZE> top_half;

      for (int pos = 0; pos < half; pos++)
      {
        bottom_half[pos] = cards[pos];
      }

      for (int pos = 0; pos < half; pos++)
      {
        top_half[pos] = cards[pos+half];
      }

      //Set up new pack with bottom and top packs
      for (int pos = 0; pos < half; pos++)
      {
        if (pos*2 >= 24)
        {
          break;
        }
        cards[pos*2] = top_half[pos];
      }

      for (int pos = 0; pos < half; pos++)
      {
        if (pos*2 + 1 >= 24)
        {
          break;
        }
        cards[pos*2 + 1] = bottom_half[pos];
      }
    }
    reset();
  }


  // EFFECTS: returns true if there are no more cards left in the pack
  bool Pack:: empty() const{
    if(next>=24){
      return true;
    }
    return false;
  }



