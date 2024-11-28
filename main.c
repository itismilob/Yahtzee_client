#include "common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 8000
#define BUFSIZE 512

int main() {
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

	result = connect(clientsock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (result == SOCKET_ERROR) {
		err_quit("connect");
	}
	printf("������ �����ϼ̽��ϴ�.\n");
	printf("�ٸ� ������ ������ ������Դϴ�...\n");

	// send, recv
	char buffer[BUFSIZE + 1];
	char user[BUFSIZE + 1];

	// ���� Ȯ��
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	strcpy(user, buffer);

	printf("����� [���� %s] �Դϴ�.\n", user);

	// ���� ����
	result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
	if (result == SOCKET_ERROR) {
		err_quit('recv');
	}
	buffer[result] = '\0';
	printf("���� ����!\n");

	while (1) {
		printf("[���� %s] %s��\n", user, buffer);

		if (strcmp(user, buffer) == 0) {
			// ������ �Է�
			printf("\n ���� ������ : ");
			fgets(buffer, BUFSIZE, stdin);

			result = send(clientsock, buffer, (int)strlen(buffer), 0);
		}

		// ���� ��� ����
		result = recv(clientsock, buffer, BUFSIZE, MSG_WAITALL);
		if (result == SOCKET_ERROR) {
			err_quit('recv');
		}
	}



	// ���� ����
	closesocket(clientsock);

	// ���� ����
	WSACleanup();
}