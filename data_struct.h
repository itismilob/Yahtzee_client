#pragma once

#define SCORE_LEN 16
#define TRUE 1
#define FALSE 0

typedef struct Users {
	int usernumber;
	SOCKET usersock;
	struct sockaddr_in useraddr;
	int useraddrlen;
} Users;

typedef struct Scoreboard {
	int userScore[MAX_USER][SCORE_LEN];
	int turn;
} Scoreboard;

/*
0 Ones
1 Twos
2 Threes
3 Fours
4 Fives
5 Sixes
6 Three of a kind
7 Four of a kind
8 Full House
9 Small Straight
10 Large Straight
11 YHATZEE
12 Chance
13 Bonus
14 Yahtzee bonus
15 Total
*/

typedef struct DiceData {
	int diceList[5];
	int isStop;
} DiceData;

typedef struct ScoreData {
	int combination;
	int score;
}ScoreData;



