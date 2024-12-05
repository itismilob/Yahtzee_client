#pragma once

#define SCORE_LEN 13
#define TRUE 1
#define FALSE 0

typedef int UserNumber; // 1, 2

typedef struct Scoreboard {
	int user1[SCORE_LEN];
	int user2[SCORE_LEN];
	int turn;
} Scoreboard;

typedef struct DiceData {
	int diceList[5];
	int isStop;
} DiceData;

typedef struct ScoreData {
	int combination;
	int score;
}ScoreData;

