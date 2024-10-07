#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

TEST(test_player_make_trump) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(NINE, DIAMONDS));
  shelly->add_card(Card(KING, DIAMONDS));
  shelly->add_card(Card(QUEEN, DIAMONDS));

  Card eight_diamonds(EIGHT, DIAMONDS);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_diamonds,
    true,           
    1,            
    trump          
  );

  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, DIAMONDS);

  delete shelly; 
}

TEST(test_player_make_trump_left_bower) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(KING, DIAMONDS));
  shelly->add_card(Card(JACK, HEARTS));
  shelly->add_card(Card(QUEEN, SPADES));

  Card eight_diamonds(EIGHT, DIAMONDS);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_diamonds,
    true,           
    1,            
    trump          
  );

  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, DIAMONDS);

  delete shelly; 
}

TEST(test_player_make_trump_second_round) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(ACE, DIAMONDS));
  shelly->add_card(Card(KING, HEARTS));
  shelly->add_card(Card(QUEEN, HEARTS));

  Card eight_diamonds(EIGHT, DIAMONDS);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_diamonds,
    false,           
    2,            
    trump          
  );

  Card eight_spades(EIGHT, SPADES);
  bool orderup_2 = shelly->make_trump(
    eight_spades,
    false,           
    2,            
    trump          
  );

  ASSERT_TRUE(orderup);
  ASSERT_FALSE(orderup_2);
  ASSERT_EQUAL(trump, HEARTS);

  delete shelly; 
}

TEST(test_player_is_dealer) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(ACE, DIAMONDS));
  shelly->add_card(Card(KING, HEARTS));
  shelly->add_card(Card(QUEEN, HEARTS));

  Card eight_spades(EIGHT, SPADES);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_spades,
    true,           
    2,            
    trump          
  );
  ASSERT_TRUE(orderup);
  ASSERT_EQUAL(trump, CLUBS);
  delete shelly; 
}

TEST(test_player_is_dealer_first_round) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(ACE, DIAMONDS));
  shelly->add_card(Card(KING, HEARTS));
  shelly->add_card(Card(QUEEN, HEARTS));

  Card eight_spades(EIGHT, SPADES);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_spades,
    true,           
    1,            
    trump          
  );
  ASSERT_FALSE(orderup);

  delete shelly; 
}

TEST(test_player_do_not_make_trump) {
  Player * shelly = Player_factory("Shelly", "Simple");
  shelly->add_card(Card(NINE, DIAMONDS));
  shelly->add_card(Card(KING, HEARTS));
  shelly->add_card(Card(QUEEN, CLUBS));

  Card eight_diamonds(EIGHT, DIAMONDS);
  Suit trump;
  bool orderup = shelly->make_trump(
    eight_diamonds,
    true,           
    1,            
    trump          
  );

  ASSERT_FALSE(orderup);
  delete shelly; 
}

TEST(test_player_add_discard)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(NINE, CLUBS));
  joe->add_card(Card(TEN, CLUBS));

  joe->add_and_discard(
    Card(NINE, HEARTS)
  );

  Card card_led = joe->lead_card(CLUBS);
  Card nine_hearts(NINE, HEARTS);

  ASSERT_EQUAL(card_led, nine_hearts);

  delete joe;
}

TEST(test_player_play_card_left_bower)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(KING, HEARTS));
  joe->add_card(Card(JACK, CLUBS));
  joe->add_card(Card(TWO, SPADES));

  joe->add_and_discard(
    Card(NINE, SPADES)
  );

  Card card_led = joe->play_card(Card(TWO, SPADES), SPADES);
  Card jack_clubs(JACK, CLUBS);

  ASSERT_EQUAL(card_led, jack_clubs);

  delete joe;
}

TEST(test_player_play_card_left_bower_and_right)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(KING, HEARTS));
  joe->add_card(Card(JACK, CLUBS));
  joe->add_card(Card(JACK, SPADES));
  joe->add_card(Card(TWO, SPADES));

  joe->add_and_discard(
    Card(NINE, SPADES)
  );

  Card card_led = joe->play_card(Card(TWO, SPADES), SPADES);
  Card jack_spades(JACK, SPADES);

  ASSERT_EQUAL(card_led, jack_spades);

  delete joe;
}

TEST(test_player_add_discard_left_bower)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(KING, SPADES));
  joe->add_card(Card(JACK, CLUBS));
  joe->add_card(Card(QUEEN, SPADES));

  joe->add_and_discard(
    Card(NINE, SPADES)
  );

  Card card_led = joe->lead_card(SPADES);
  Card jack_clubs(JACK, CLUBS);

  ASSERT_EQUAL(card_led, jack_clubs);

  delete joe;
}

TEST(test_player_add_discard_self)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(NINE, DIAMONDS));
  joe->add_card(Card(TEN, DIAMONDS));

  joe->add_and_discard(
    Card(EIGHT, DIAMONDS)
  );

  Card nine_clubs = Card(NINE, CLUBS);
  Card card_played = joe->play_card(nine_clubs, SPADES);
  ASSERT_EQUAL(card_played, Card(NINE, DIAMONDS));
  delete joe;
}

TEST(test_player_add_discard_keep_small)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(NINE, DIAMONDS));
  joe->add_card(Card(TEN, DIAMONDS));

  joe->add_and_discard(
    Card(EIGHT, HEARTS)
  );

  Card nine_clubs = Card(NINE, CLUBS);
  Card card_played = joe->play_card(nine_clubs, SPADES);
  ASSERT_EQUAL(card_played, Card(EIGHT, HEARTS));
  delete joe;
}

TEST(test_player_add_discard_regular)
{
  Player * joe = Player_factory("Joe", "Simple");
  joe->add_card(Card(TEN, DIAMONDS));

  joe->add_and_discard(
    Card(EIGHT, HEARTS)
  );

  Card nine_diamonds = Card(NINE, DIAMONDS);
  Card card_played = joe->play_card(nine_diamonds, DIAMONDS);
  ASSERT_EQUAL(card_played, Card(EIGHT, HEARTS));
  delete joe;
}

TEST(test_player_play_card)
{
  Player * sally = Player_factory("Sally", "Simple");
  sally->add_card(Card(TWO, DIAMONDS));
  sally->add_card(Card(KING, CLUBS));
  sally->add_card(Card(ACE, HEARTS));

  Card eight_diamonds(EIGHT, DIAMONDS);
  Card card_played = sally->play_card(
    eight_diamonds, 
    HEARTS
  ); 
  
  ASSERT_EQUAL(card_played, Card(TWO, DIAMONDS));
  delete sally;
}

TEST(test_player_play_card_with_trump)
{
  Player * sally = Player_factory("Sally", "Simple");
  sally->add_card(Card(TWO, SPADES));
  sally->add_card(Card(FIVE, DIAMONDS));
  sally->add_card(Card(EIGHT, DIAMONDS));

  Card eight_clubs(EIGHT, CLUBS);
  Card card_played = sally->play_card(
    eight_clubs, 
    SPADES
  ); 

  ASSERT_EQUAL(card_played, Card(FIVE, DIAMONDS));
  delete sally;
}

TEST (test_player_lead_card_with_trump)
{
  Player * fredrick = Player_factory("Fredrick", "Simple");
  fredrick->add_card(Card(KING, DIAMONDS));
  fredrick->add_card(Card(QUEEN, DIAMONDS));
  fredrick->add_card(Card(TEN, CLUBS));
  fredrick->add_card(Card(JACK, SPADES));

  Card card_led = fredrick->lead_card(DIAMONDS);
  Card jack_spades(JACK, SPADES);
  ASSERT_EQUAL(card_led, jack_spades);

  Card card_led_2 = fredrick->lead_card(CLUBS);
  Card king_diamonds(KING, DIAMONDS);
  ASSERT_EQUAL(card_led_2, king_diamonds);

  delete fredrick;
}

TEST_MAIN()
