#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <iostream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here
TEST(test_card_ctor_basic){
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(test_is_face_or_ace){
    Card c(KING, DIAMONDS);
    ASSERT_EQUAL(c.is_face_or_ace(), true);

    Card c2(FIVE, CLUBS);
    ASSERT_EQUAL(c2.is_face_or_ace(), false);

    Card c3(ACE, SPADES);
    ASSERT_EQUAL(c3.is_face_or_ace(), true);
}

TEST(test_is_right_bower){
    Card c1 = Card(JACK, CLUBS);
    ASSERT_EQUAL(c1.is_right_bower(CLUBS), true);

    Card c2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(c2.is_right_bower(HEARTS), false);

    Card c3 = Card(JACK, HEARTS);
    ASSERT_EQUAL(c3.is_right_bower(DIAMONDS), false);

    Card c4 = Card(ACE, CLUBS);
    ASSERT_EQUAL(c4.is_right_bower(DIAMONDS), false);
}

TEST(test_is_left_bower){
    Card c1 = Card(JACK, DIAMONDS);
    ASSERT_EQUAL(c1.is_left_bower(HEARTS), true);
    
    Card c2 = Card(KING, SPADES);
    ASSERT_EQUAL(c2.is_left_bower(CLUBS), false);

    Card c3 = Card(JACK, HEARTS);
    ASSERT_EQUAL(c3.is_left_bower(CLUBS), false);

    Card c4 = Card(JACK, SPADES);
    ASSERT_EQUAL(c4.is_left_bower(SPADES), false);
}

TEST(test_is_trump){
    Card c1 = Card(KING, CLUBS);
    ASSERT_EQUAL(c1.is_trump(CLUBS), true);

    Card c2 = Card(TWO, SPADES);
    ASSERT_EQUAL(c2.is_trump(HEARTS), false);

    Card c3 = Card(JACK, DIAMONDS);
    ASSERT_EQUAL(c3.is_trump(HEARTS), true);
}

TEST(test_less_than){
    Card c1 = Card(TEN, CLUBS);
    Card c2 = Card(ACE, CLUBS);
    ASSERT_EQUAL(c1<c2, true);

    Card c3 = Card(TEN, SPADES);
    Card c4 = Card(TEN, DIAMONDS);
    ASSERT_EQUAL(c3>c4, false);
}

TEST(test_less_than_equal_to){
    Card c1 = Card(QUEEN, HEARTS);
    Card c2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(c1<=c2, true);

    Card c3 = Card(TEN, DIAMONDS);
    Card c4 = Card(TEN, SPADES);
    ASSERT_EQUAL(c3<=c4, false);

    Card c5 = Card(JACK, CLUBS);
    Card c6 = Card(TEN, SPADES);
    ASSERT_EQUAL(c5<=c6, false);
}

TEST(test_greater_than){
    Card c1 = Card(ACE, CLUBS);
    Card c2 = Card(TEN, CLUBS);
    ASSERT_EQUAL(c1>c2, true);

    Card c3 = Card(TEN, DIAMONDS);
    Card c4 = Card(TEN, SPADES);
    ASSERT_EQUAL(c4>c3, false);
}

TEST(test_greater_than_equal_to){
    Card c1 = Card(QUEEN, HEARTS);
    Card c2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(c1>=c2, true);

    Card c3 = Card(TEN, DIAMONDS);
    Card c4 = Card(TEN, SPADES);
    ASSERT_EQUAL(c3>=c4, true);

    Card c5 = Card(JACK, CLUBS);
    Card c6 = Card(TEN, SPADES);
    ASSERT_EQUAL(c5>=c6, true);
}

TEST(test_equal_to){
    Card c1 = Card(QUEEN, HEARTS);
    Card c2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(c1==c2, true);

    Card c3 = Card(NINE, CLUBS);
    Card c4 = Card(TEN, CLUBS);
    ASSERT_EQUAL(c3==c4, false);
}

TEST(test_not_equal_to){
    Card c1 = Card(QUEEN, HEARTS);
    Card c2 = Card(QUEEN, HEARTS);
    ASSERT_EQUAL(c1!=c2, false);

    Card c3 = Card(NINE, CLUBS);
    Card c4 = Card(TEN, CLUBS);
    ASSERT_EQUAL(c3!=c4, true);
}


TEST(test_Suit_next){
    Suit s1 = SPADES;
    ASSERT_EQUAL(Suit_next(s1), CLUBS);

    Suit s2 = HEARTS;
    ASSERT_EQUAL(Suit_next(s2), DIAMONDS);
}

TEST(test_Card_less_one){
    Card c1 = Card(ACE, SPADES);
    Card c2 = Card(NINE, HEARTS);
    Suit t1 = HEARTS;
    ASSERT_EQUAL(Card_less(c1, c2, t1), true);

    Card c3 = Card(NINE, DIAMONDS);
    Card c4 = Card(QUEEN, DIAMONDS);
    Suit t2 = DIAMONDS;
    ASSERT_EQUAL(Card_less(c3, c4, t2), true);

    Card c5 = Card(KING, HEARTS);
    Card c6 = Card(KING, HEARTS);
    Suit t3 = SPADES;
    ASSERT_EQUAL(Card_less(c5, c6, t3), false);

    Card c7 = Card(JACK, HEARTS);
    Card c8 = Card(KING, HEARTS);
    Suit t4 = DIAMONDS;
    ASSERT_EQUAL(Card_less(c7, c8, t4), false);
}

TEST(test_get_suit)
{
    Suit trump = HEARTS;
    Card c1 = Card(JACK, DIAMONDS);
    
    ASSERT_EQUAL(c1.get_suit(trump), HEARTS);
}

TEST(test_Card_less_two){
    Card c1 = Card(ACE, CLUBS);
    Card c2 = Card(ACE, SPADES);
    Card L1 = Card(NINE, SPADES);
    Suit t1 = CLUBS;
    ASSERT_EQUAL(Card_less(c1, c2, L1, t1), false);

    Card c3 = Card(ACE, SPADES);
    Card c4 = Card(ACE, CLUBS);
    Card L2 = Card(NINE, CLUBS);
    Suit t2 = SPADES;
    ASSERT_EQUAL(Card_less(c3, c4, L2, t2), false);

    Card c5 = Card(KING, HEARTS);
    Card c6 = Card(KING, HEARTS);
    Card L3 = Card(KING, DIAMONDS);
    Suit t3 = HEARTS;
    ASSERT_EQUAL(Card_less(c5, c6, L3, t3), false);

    Card c7 = Card(NINE, SPADES);
    Card c8 = Card(JACK, SPADES);
    Card L4 = Card(NINE, HEARTS);
    Suit t4 = SPADES;
    ASSERT_EQUAL(Card_less(c7, c8, L4, t4), true);

    Card c9 = Card(NINE, HEARTS);
    Card c10 = Card(JACK, SPADES);
    Card L5 = Card(NINE, HEARTS);
    Suit t5 = CLUBS;
    ASSERT_EQUAL(Card_less(c9, c10, L5, t5), true);
}

TEST(test_specific_case){
    Card c1 = Card(QUEEN, HEARTS);
    Card c2 = Card(KING, HEARTS);
    Card L1 = Card(QUEEN, HEARTS);
    Suit t1 = HEARTS;
    ASSERT_EQUAL(Card_less(c1, c2, L1, t1), true);
}

TEST_MAIN()
