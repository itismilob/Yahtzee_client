#include "common.h"
#include "data_struct.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
	// ���⼭ ���� ���� �����ְ� ���� �����ϰ� �ϱ�



	int result;

	// ���� �ʱ�ȭ
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) return 1;

	// ���� ����
	SOCKET clientsock = socket(AF_INET, SOCK_STREAM, 0);
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
	printf("������ �����ϼ̽��ϴ�.\n");
	printf("�ٸ� ������ ������ ������Դϴ�...\n");


	// ���� ���� �ʱ�ȭ #1-3
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


	// ���� Ȯ�� #1-4 recv UserNumber
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	usernumber = (int)buffer;
	printf("����� [���� %d] �Դϴ�.\n", usernumber);

	// ���� ���� #1-5 recv Scoreboard
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	printf("���� ����!\n");

	// #2
	while (1) {
		printf("[���� %d] %s��\n", usernumber, buffer);

		// ���̸� �ֻ����� ������ ������ ����
		if (usernumber == (int)buffer) {
			// �ֻ����� ���� ������ ������ ���� #2-1 send DiceData

			fgets(buffer, BUFSIZE, stdin);

			result = send(clientsock, buffer, (int)strlen(buffer), 0);


			// �ֻ����� Ȯ���Ǹ� ������ ���� #2-3 send ScoreData


		}
		// ���� �ƴϸ� �ٸ� ������ �ֻ��� Ȯ��
		else {
			// �ֻ��� Ȯ�� #2-2 recv DiceData (isStop)
			result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
			if (result == SOCKET_ERROR) {
				err_quit('recv');
			}
		}


		// ���� ����� �������� ���� #2-3-* recv Scoreboard (����)
	}



	// ���� ����
	closesocket(clientsock);

	// ���� ����
	WSACleanup();
}