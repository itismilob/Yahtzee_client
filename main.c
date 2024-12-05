#include "common.h"
#include "data_struct.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
	// 여기서 게임 설명 보여주고 게임 접속하게 하기



	int result;

	// 윈속 초기화
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// 소켓 생성
	SOCKET clientsock = socket(AF_INET, SOCK_STREAM, 0);
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
	printf("서버에 접속하셨습니다.\n");
	printf("다른 유저의 접속을 대기중입니다...\n");


	// 게임 변수 초기화 #1-3
	char buffer[BUFSIZE + 1];

	UserNumber usernumber;
	int i;

	DiceData diceData;
	for (i = 0;i < 5;i++) {
		diceData.diceList[i] = NULL;
	}
	diceData.isStop = FALSE;

	ScoreData scoreData;
	scoreData.combination = NULL;
	scoreData.score = 0;

	Scoreboard scoreboard;
	scoreboard.turn = 1;
	for (i = 0;i < SCORE_LEN;i++) {
		scoreboard.user1[i] = 0;
		scoreboard.user2[i] = 0;
	}


	// 유저 확정 #1-4 recv UserNumber
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	usernumber = (int)buffer;
	printf("당신은 [유저 %d] 입니다.\n", usernumber);

	// 게임 시작 #1-5 recv Scoreboard
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	printf("게임 시작!\n");

	// #2
	while (1) {
		printf("[유저 %d] %s턴\n", usernumber, buffer);

		// 턴이면 주사위를 굴려서 서버로 전송
		if (usernumber == (int)buffer) {
			// 주사위를 굴릴 때마다 서버로 전송 #2-1 send DiceData

			fgets(buffer, BUFSIZE, stdin);

			result = send(clientsock, buffer, (int)strlen(buffer), 0);


			// 주사위가 확정되면 서버로 전송 #2-3 send ScoreData


		}
		// 턴이 아니면 다른 유저의 주사위 확인
		else {
			// 주사위 확인 #2-2 recv DiceData (isStop)
			result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
			if (result == SOCKET_ERROR) {
				err_quit('recv');
			}
		}


		// 점수 결과를 서버에서 받음 #2-3-* recv Scoreboard (공통)
	}



	// 소켓 종료
	closesocket(clientsock);

	// 윈속 종료
	WSACleanup();
}