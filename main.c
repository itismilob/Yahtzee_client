
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...

#include "common.h"
#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define SERVERPORT 8000
#define BUFSIZE 512

// �ִ� ���� ���� �� 2~
#define MAX_USER 2

// �ִ� ���� 13
#define MAX_ROUND 2

#include "data_struct.h"

char* SERVERIP = (char*)"127.0.0.1";

int result;
char buffer[BUFSIZE + 1];

SOCKET clientsock;
// ���� ����
int usernumber;
DiceData diceData;
ScoreData scoreData;
Scoreboard scoreboard;

#include "data_connection.h"
#include "game.h"





//ó�� �����Ҷ� �ߵ��Ǵ� �͵� �Դϴ�.
int main() {
	// �׽�Ʈ��
	/*resetGame();
	connect_game();
	return;*/

	while (1) {
		clear();
		printf("Welcome to Yahtzee!\n");
		printf("���� ���ʹٸ� \"rule\"�� �Է��ϼ���.\n");
		printf("������ �����Ϸ��� \"start\"�� �Է��ϼ���.\n");
		printf("������ �����Ϸ��� \"quit\"�� �Է��ϼ���.\n");

		choice();

		if (strcmp(says, "rule") == 0) {
			showRules();
		}
		else if (strcmp(says, "start") == 0) {
			resetGame();
			connect_game();
		}
		else if (strcmp(says, "quit") == 0) {
			printf("\n\n\n������ �����մϴ�.");
			break;
		}
		else {
			printf("�߸� �Է��߽��ϴ�.\n\n");
		}
	}

	// ���� ����
	closesocket(clientsock);

	// ���� ����
	WSACleanup();
}


int connect_game() {
	int i, j;

	// ���� �ʱ�ȭ
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// ���� ����
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsock == INVALID_SOCKET) err_quit("���� ����");

	// connect
	// ���� �ּ� ����ü ����
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
	printf("������ �����ϼ̽��ϴ�.\n");
	printf("�ٸ� ������ ������ ������Դϴ�...\n");


	// ���� ���� �ʱ�ȭ #1-3
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


	// ���� Ȯ�� #1-4 recv UserNumber
	result = recv(clientsock, (char*)&usernumber, sizeof(usernumber), MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit("recv usernumber");
	}
	usernumber = ntohl(usernumber);
	clear();
	printf("����� [���� %d] �Դϴ�.\n", usernumber+1);


	printf("\n���� ����!\n");
	// ���� ���� #1-5 recv Scoreboard
	recvScoreboard(clientsock);
	showScoreboard();

	int round = 0;
	// #2
	while(round < MAX_ROUND) {
		printf("[���� %d] ���� %d���Դϴ�.\n", usernumber + 1, scoreboard.turn + 1);

		// ���̸� �ֻ����� ������ ������ ����
		if (usernumber == scoreboard.turn) {
			// �ֻ����� ���� ������ ������ ���� #2-1 send DiceData
			gameRound();

			// �ֻ����� Ȯ���Ǹ� ������ ���� #2-3 send ScoreData
			sendScoreData(clientsock);
		}
		// ���� �ƴϸ� �ٸ� ������ �ֻ��� Ȯ��
		else {
			// �ֻ��� Ȯ�� #2-2 recv DiceData (isStop)
			do {
				printf("�ٸ� ������ ����� ��ٸ�����...\n");

				recvDiceData(clientsock);

				clear();
				showScoreboard();
				printf("[���� %d] diceData [ ", scoreboard.turn + 1);
				for (i = 0;i < 5;i++) {
					printf("%d ", diceData.diceList[i]);
				}
				printf("]\n");
				
			} while (!diceData.isStop);

			recvScoreData(clientsock);
		}

		clear();
		printf("������ �����Ͽ����ϴ�.\n[%d] %s - %d\n\n", scoreData.combination+1, combiList[scoreData.combination], scoreData.score);

		recvScoreboard(clientsock);
		showScoreboard();


		if (scoreboard.turn == 0) round++;
	}

	// ���� ����� ���� ��ο��� ���� #2-3-* send Scoreboard
	recvScoreboard(clientsock);
	clear();
	showScoreboard();
	printf("������ ����Ǿ����ϴ�.\n");

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
		printf("�����Դϴ�.\n");
	}
	printf("���ڴ� ���� ");
	for (i = 0;i < MAX_USER;i++) {
		if (winner[i] == 1) {
			printf("%d�� ", i + 1);
		}
	}
	printf("�Դϴ�!\n\n");

	printf("���� �޴��� ���ư��÷��� �ƹ� ���ڳ� �Է��ϼ���.\n");
	choice();
}