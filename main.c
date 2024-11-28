#include "common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
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

	result = connect(clientsock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (result == SOCKET_ERROR) {
		err_quit("connect");
	}
	printf("서버에 접속하셨습니다.\n");
	printf("다른 유저의 접속을 대기중입니다...\n");

	// send, recv
	char buffer[BUFSIZE + 1];
	char user[BUFSIZE + 1];

	// 유저 확정
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	strcpy(user, buffer);

	printf("당신은 [유저 %s] 입니다.\n", user);

	// 게임 시작
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	printf("게임 시작!\n");

	while (1) {
		printf("[유저 %s] %s턴\n", user, buffer);

		if (strcmp(user, buffer) == 0) {
			// 데이터 입력
			printf("\n 보낼 데이터 : ");
			fgets(buffer, BUFSIZE, stdin);

			result = send(clientsock, buffer, (int)strlen(buffer), 0);
		}

		// 게임 결과 수신
		result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
		if (result == SOCKET_ERROR) {
			err_quit('recv');
		}
	}



	// 소켓 종료
	closesocket(clientsock);

	// 윈속 종료
	WSACleanup();
}