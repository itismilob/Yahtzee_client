#pragma once

Scoreboard tempboard;
DiceData tempdice;
ScoreData tempscore;

void sendScoreboard(SOCKET socket) {
	int i, j;
	for (i = 0;i < MAX_USER;i++) {
		for (j = 0;j < SCORE_LEN;j++) {
			tempboard.userScore[i][j] = htonl(scoreboard.userScore[i][j]);
		}
	}
	tempboard.turn = htonl(scoreboard.turn);
	result = send(socket, (char*)&tempboard, sizeof(tempboard), 0);
	if (result == SOCKET_ERROR) {
		err_quit("sendScoreboard");
	}
}

void recvScoreboard(SOCKET socket) {
	int i, j;
	result = recv(socket, (char*)&tempboard, sizeof(tempboard), MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit("recvScoreboard");
	}
	for (i = 0;i < MAX_USER;i++) {
		for (j = 0;j < SCORE_LEN;j++) {
			scoreboard.userScore[i][j] = ntohl(tempboard.userScore[i][j]);
		}
	}
	scoreboard.turn = htonl(tempboard.turn);
}


void sendDiceData(SOCKET socket) {
	int i;
	for (i = 0;i < 5;i++) {
		tempdice.diceList[i] = htonl(diceData.diceList[i]);
	}
	tempdice.isStop = htonl(diceData.isStop);
	result = send(socket, (char*)&tempdice, sizeof(tempdice), 0);
	if (result == SOCKET_ERROR) {
		err_quit("sendDiceData");
	}
}

void recvDiceData(SOCKET socket) {
	int i;
	result = recv(socket, (char*)&tempdice, sizeof(tempdice), MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit("recvDiceData");
	}
	for (i = 0;i < 5;i++) {
		diceData.diceList[i] = ntohl(tempdice.diceList[i]);
	}
	diceData.isStop = ntohl(tempdice.isStop);
}

void sendScoreData(SOCKET socket) {
	tempscore.combination = htonl(scoreData.combination);
	tempscore.score = htonl(scoreData.score);
	result = send(socket, (char*)&tempscore, sizeof(tempscore), 0);
	if (result == SOCKET_ERROR) {
		err_quit("sendScoreData");
	}
}

void recvScoreData(SOCKET socket) {
	result = recv(socket, (char*)&tempscore, sizeof(tempscore), MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit("recvScoreData");
	}
	scoreData.combination = ntohl(tempscore.combination);
	scoreData.score = ntohl(tempscore.score);
}