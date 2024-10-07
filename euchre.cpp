#include <iostream>
#include <vector>
#include <fstream>
#include <istream>
#include <cassert>
#include <string>
#include "Pack.hpp"
#include "Player.hpp"
#include "Card.hpp"

using namespace std;

class Game {
    private:
    Pack* pack;
    Suit trump;
    Card upcard;
    bool shuffle;
    bool playing;
    int points;
    int orderUpTeam;

    std::vector<Player*> players;

    int team1_points;
    int team2_points;

    int team1_tricks = 0;
    int team2_tricks = 0;

    int hand;
    int dealer;

    void shuffle_cards(){
        if (!shuffle){
            pack->reset();
            return;
        }
        pack->shuffle();
    }

    void deal(){
        cout<<players[dealer]->get_name() << " deals"<< endl;
        int dealNum = dealer;

        //first batch
        dealNum = dealer + 1;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<3;i++){
            players[dealNum]->add_card(pack->deal_one());
        }

        dealNum = dealer + 2;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<2;i++){
           players[dealNum]->add_card(pack->deal_one());
        }

        dealNum = dealer + 3;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<3;i++){
            players[dealNum]->add_card(pack->deal_one());
        }

        for(int i=0;i<2;i++){
            players[dealer]->add_card(pack->deal_one());
        }

        //second batch

        dealNum = dealer + 1;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<2;i++){
            players[dealNum]->add_card(pack->deal_one());
        }

        dealNum = dealer + 2;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<3;i++){
            players[dealNum]->add_card(pack->deal_one());
        }

        dealNum = dealer + 3;
        if (dealNum > 3){
            dealNum -= 4;
        }
        for(int i=0;i<2;i++){
            players[dealNum]->add_card(pack->deal_one());
        }

        for(int i=0;i<3;i++){
            players[dealer]->add_card(pack->deal_one());
        }
    }

    int make_trump(){
        bool madeTrump = false;
        for (int i = 1; i <= 3; i++){
            int playerNum = dealer + i;
            if (playerNum > 3){
                playerNum -= 4;
            }
            madeTrump =
                (*players[playerNum]).make_trump(upcard, false, 1, trump);
            if (madeTrump)
            {
                cout << players[playerNum]->get_name() <<
                     " orders up " << trump << endl;
                (*players[dealer]).add_and_discard(upcard);
                return playerNum;
            }
            cout << players[playerNum]->get_name() << " passes" << endl;
        }

        madeTrump = 
            (*players[dealer]).make_trump(upcard, true, 1, trump);
        if (madeTrump){
            cout << (*players[dealer]).get_name() << " orders up " << trump << endl;
            (*players[dealer]).add_and_discard(upcard);
            return dealer;
        }
        cout << (*players[dealer]).get_name() << " passes" << endl;

        // Round 2
        for (int i = 1; i <= 3; i++){
            int playerNum = dealer + i;
            if (playerNum > 3){
                playerNum -= 4;
            }
            madeTrump = 
                players[playerNum]->make_trump(upcard, false, 2, trump);
            if (madeTrump)
            {
                cout << players[playerNum]->get_name() <<
                    " orders up " << trump << endl;
                return playerNum;
            }
            cout << (*players[playerNum]).get_name() << " passes" << endl;
        }
        (*players[dealer]).make_trump(upcard, true, 2, trump);
        cout << players[dealer]->get_name() << " orders up " << trump << endl;
        return dealer;
    }

    int trick(int leaderInt)
    {
        std::vector<Card> cards;
        std::vector<int> playerOrder;

        cards.clear();
        playerOrder.clear();

        upcard = players[leaderInt]->lead_card(trump);
        cout << upcard << " led by " << players[leaderInt]->get_name() << endl;

        cards.push_back(upcard);
        playerOrder.push_back(leaderInt);

        for (int i = 1; i < 4; i++)
        {
            int player = leaderInt + i;
            if (player > 3){
                player = player - 4;
            }
            Card played = players[player]->play_card(upcard, trump);
            cout << played << " played by " << players[player]->get_name() << endl;

            cards.push_back(played);
            playerOrder.push_back(player);
        }

        Card largest = cards[0];
        int winnerNum = playerOrder[0];

        for (int i = 1; i < cards.size(); i++)
        {
            if (Card_less(largest, cards[i], upcard, trump))
            {
                largest = cards[i];
                winnerNum = playerOrder[i];
            }
        }

        if (winnerNum == 0 || winnerNum == 2){
            team1_tricks++;
        }
        else{
            team2_tricks++;
        }

        cout << players[winnerNum]->get_name() << " takes the trick" << endl;
        cout << endl;

        return winnerNum;
    } 

    void announce_winner()
    {
        if (team1_tricks > team2_tricks)
        {
            cout << players[0]->get_name() << " and " << players[2]->get_name() 
                << " win the hand" << endl;

            if (orderUpTeam == 1 && team1_tricks > 4) 
            {
                team1_points += 2;
                cout << "march!" << endl;
            }
            else if (orderUpTeam == 2)
            {
                team1_points += 2;
                cout << "euchred!" << endl;
            }
            else
            {
                team1_points += 1;
            }
        }
        else
        {
            cout << players[1]->get_name() << " and " << players[3]->get_name() 
                << " win the hand" << endl;

            if (orderUpTeam == 2 && team2_tricks > 4) 
            {
                team2_points += 2;
                cout << "march!" << endl;
            }
            else if (orderUpTeam == 1)
            {
                team2_points += 2;
                cout << "euchred!" << endl;
            }
            else
            {
                team2_points += 1;
            }
        }
    }  

    void play_hand()
    {
        shuffle_cards();

        cout << "Hand " << hand << endl;
        deal();

        upcard = pack->deal_one();
        cout << upcard << " turned up" << endl;

        int orderUpPlayer = make_trump();
        cout << endl;

        if (orderUpPlayer == 0 || orderUpPlayer == 2) {
            orderUpTeam = 1;
        }
        else {
            orderUpTeam = 2;
        }

        team1_tricks = 0;
        team2_tricks = 0;

        int leaderInt = dealer + 1;
        if (leaderInt > 3){
            leaderInt -= 4;
        }
        for(int i = 0; i < 5; i++)
        {
            int winnerNum = trick(leaderInt);
            leaderInt = winnerNum;
        }
        announce_winner();
    };
    // ...

    public:
    Game(Pack* pack, bool shuffle, int points, std::vector<Player*> players) 
    : pack(pack), shuffle(shuffle), points(points), players(players)
    { }
    
    void play()
    {
        hand = 0;
        dealer = 0;

        team1_points = 0;
        team2_points = 0;

        playing = true;
        while (playing)
        {
            play_hand();
            cout << players[0]->get_name() << " and " << players[2]->get_name() 
                << " have " << team1_points << " points" << endl;

            cout << players[1]->get_name() << " and " << players[3]->get_name() 
                << " have " << team2_points << " points" << endl;
            cout << endl;

            hand++;
            dealer++;
            if (dealer > 3)
            {
                dealer = 0;
            }

            if (team1_points >= points)
            {
                cout << players[0]->get_name() << " and " << 
                    players[2]->get_name() << " win!" << endl;
                playing = false;
            }
            else if (team2_points >= points)
            {
                cout << players[1]->get_name() << " and " <<
                    players[3]->get_name() << " win!" << endl;
                playing = false;
            }
        }
    };
};

int main(int argc, char **argv){
    if(argc != 12){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    try{
        stoi(argv[3]);
    }
    catch(...){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    if(stoi(argv[3])<1 || stoi(argv[3])>100){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    if((string(argv[2])!="shuffle" && string(argv[2])!="noshuffle") ||
    (string(argv[5])!="Simple" && string(argv[5])!="Human") || 
    (string(argv[7])!="Simple" && string(argv[7])!="Human") || 
    (string(argv[9])!="Simple" && string(argv[9])!="Human") || 
    (string(argv[11])!="Simple" && string(argv[11])!="Human")){
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
        return 1;
    }

    string pack_input = argv[1];
    ifstream fin(pack_input);

    if (!fin.is_open()){
        cout << "Error opening file: " << pack_input << endl;
        return 2;
    }

    Pack* pack = new Pack(fin);
    bool shuffle = false;
    if (string(argv[2]) == "shuffle")
    {
        shuffle = true;
    }
    int points = stoi(argv[3]);

    std::vector<Player*> players;
    players.push_back(Player_factory(argv[4], argv[5]));
    players.push_back(Player_factory(argv[6], argv[7]));
    players.push_back(Player_factory(argv[8], argv[9]));
    players.push_back(Player_factory(argv[10], argv[11]));
    for (int i = 0; i < 12; i++){
        cout << string(argv[i]) << " ";
    }
    cout << endl;
    Game *current = new Game(pack, shuffle, points, players);
    current->play();
    delete current;
    delete pack;

    for (size_t i = 0; i < players.size(); i++){
        delete players[i];
    }
}