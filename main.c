
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...

#include "common.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#define SERVERPORT 8000
#define BUFSIZE 512

// 최대 유저 접속 수 2~
#define MAX_USER 2

// 최대 라운드 13
#define MAX_ROUND 2

#include "data_struct.h"

char* SERVERIP = (char*)"127.0.0.1";

int result;
char buffer[BUFSIZE + 1];

SOCKET clientsock;
// 게임 변수
int usernumber;
DiceData diceData;
ScoreData scoreData;
Scoreboard scoreboard;

#include "data_connection.h"
#include "game.h"





//처음 시작할때 발동되는 것들 입니다.
int main() {
	// 테스트용
	/*resetGame();
	connect_game();
	return;*/

	while (1) {
		clear();
		printf("Welcome to Yahtzee!\n");
		printf("룰을 듣고싶다면 \"rule\"을 입력하세요.\n");
		printf("게임을 시작하려면 \"start\"를 입력하세요.\n");
		printf("게임을 종료하려면 \"quit\"을 입력하세요.\n");

		choice();

		if (strcmp(says, "rule") == 0) {
			showRules();
		}
		else if (strcmp(says, "start") == 0) {
			resetGame();
			connect_game();
		}
		else if (strcmp(says, "quit") == 0) {
			printf("\n\n\n게임을 종료합니다.");
			break;
		}
		else {
			printf("잘못 입력했습니다.\n\n");
		}
	}

	// 소켓 종료
	closesocket(clientsock);

	// 윈속 종료
	WSACleanup();
}


int connect_game() {
	int i, j;

	// 윈속 초기화
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// 소켓 생성
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsock == INVALID_SOCKET) err_quit("소켓 생성");

	// connect
	// 서버 주소 구조체 생성
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	// #1-2
	result = connect(clientsock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (result == SOCKET_ERROR) {
		err_quit("connect");
	}
	clear();
	printf("서버에 접속하셨습니다.\n");
	printf("다른 유저의 접속을 대기중입니다...\n");


	// 게임 변수 초기화 #1-3
	for (i = 0;i < 5;i++) {
		diceData.diceList[i] = 0;
	}
	diceData.isStop = FALSE;

	scoreData.combination = 0;
	scoreData.score = 0;

	scoreboard.turn = 0;
	for (i = 0;i < MAX_USER;i++) {
		for (j = 0;j < SCORE_LEN;j++) {
			scoreboard.userScore[i][j] = -1;
		}
	}


	// 유저 확정 #1-4 recv UserNumber
	result = recv(clientsock, (char*)&usernumber, sizeof(usernumber), MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit("recv usernumber");
	}
	usernumber = ntohl(usernumber);
	clear();
	printf("당신은 [유저 %d] 입니다.\n", usernumber+1);


	printf("\n게임 시작!\n");
	// 게임 시작 #1-5 recv Scoreboard
	recvScoreboard(clientsock);
	showScoreboard();

	int round = 0;
	// #2
	while(round < MAX_ROUND) {
		printf("[유저 %d] 유저 %d턴입니다.\n", usernumber + 1, scoreboard.turn + 1);

		// 턴이면 주사위를 굴려서 서버로 전송
		if (usernumber == scoreboard.turn) {
			// 주사위를 굴릴 때마다 서버로 전송 #2-1 send DiceData
			gameRound();

			// 주사위가 확정되면 서버로 전송 #2-3 send ScoreData
			sendScoreData(clientsock);
		}
		// 턴이 아니면 다른 유저의 주사위 확인
		else {
			// 주사위 확인 #2-2 recv DiceData (isStop)
			do {
				printf("다른 유저의 결과를 기다리는중...\n");

				recvDiceData(clientsock);

				clear();
				showScoreboard();
				printf("[유저 %d] diceData [ ", scoreboard.turn + 1);
				for (i = 0;i < 5;i++) {
					printf("%d ", diceData.diceList[i]);
				}
				printf("]\n");
				
			} while (!diceData.isStop);

			recvScoreData(clientsock);
		}

		clear();
		printf("조합을 선택하였습니다.\n[%d] %s - %d\n\n", scoreData.combination+1, combiList[scoreData.combination], scoreData.score);

		recvScoreboard(clientsock);
		showScoreboard();


		if (scoreboard.turn == 0) round++;
	}

	// 점수 결과를 유저 모두에게 전송 #2-3-* send Scoreboard
	recvScoreboard(clientsock);
	clear();
	showScoreboard();
	printf("게임이 종료되었습니다.\n");

	int maxscore = 0;
	for (i = 0;i < MAX_USER;i++) {
		if (scoreboard.userScore[i][15] > maxscore) {
			maxscore = scoreboard.userScore[i][15];
		}
	}

	int winner[MAX_USER] = { 0 };
	int winnerCount = 0;
	for (i = 0;i < MAX_USER;i++) {
		if (scoreboard.userScore[i][15] == maxscore) {
			winner[i] = 1;
			winnerCount++;
		}
	}

	printf("\n");
	if (winnerCount > 1) {
		printf("동점입니다.\n");
	}
	printf("승자는 유저 ");
	for (i = 0;i < MAX_USER;i++) {
		if (winner[i] == 1) {
			printf("%d번 ", i + 1);
		}
	}
	printf("입니다!\n\n");

	printf("메인 메뉴로 돌아가시려면 아무 글자나 입력하세요.\n");
	choice();
}