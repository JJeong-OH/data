#include "../Common.h"
#include "../ValidateIP.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE 512


int main(int argc, char* argv[]) {
    int retval;

    // ������ �μ��� ������ IP �ּҷ� ���
    if (argc > 1) {
        if (validateIP(argv[1])) SERVERIP = argv[1];
        else printf("������ IP �ּҰ� ��ȿ���� �ʽ��ϴ�!\n �⺻ ���� �ּҸ� ����մϴ� !~ \n");
    }

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // ���� ����
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // ���� �ּ� ����ü �ʱ�ȭ
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);

    // ������ ��ſ� ����� ����
    struct sockaddr_in peeraddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    int len;

    // ������ ������ ���
    while (1) {
        // ������ �Է�
        printf("\n[���� ������] ");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

        // '\n' ���� ����
        len = (int)strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        // ������ ������
        retval = sendto(sock, buf, (int)strlen(buf), 0, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) { err_display("sendto()"); break; }
        printf("[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

        // ������ �ޱ�
        addrlen = sizeof(peeraddr);
        retval = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr*)&peeraddr, &addrlen);
        if (retval == SOCKET_ERROR) { err_display("recvfrom()"); break; }

        // �۽����� �ּ� üũ
        if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr)) != 0) {
            printf("[����] �߸��� �������Դϴ�!\n");
            break;
        }

        // ���� ������ ���
        buf[retval] = '\0';
        printf("[���� ����] %s (%d bytes)\n", buf, retval);
    }

    // ���� �ݱ�
    closesocket(sock);

    // ���� ����
    WSACleanup();

    return 0;
}