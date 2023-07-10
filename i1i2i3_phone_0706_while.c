#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#define N 16384

int main(int argc, char *argv[]) {
    if (argc > 3) {
        printf("Usage: %s <port> or %s <IP> <port>\n", argv[0], argv[0]);
        return 1;
    }

    int s;
    int ss;

    // サーバ側の処理
    if (argc == 2) {
        int port = atoi(argv[1]);

        ss = socket(AF_INET, SOCK_STREAM, 0);
        if (ss < 0) {
            perror("socket");
            return 1;
        }

        struct sockaddr_in serv_addr;
        memset(&serv_addr, 0, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        serv_addr.sin_addr.s_addr = INADDR_ANY; // 「どこにでもいい」の意

        if (bind(ss, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("bind"); close(ss); exit(1);
        }

        if (listen(ss, 10) < 0) {
            perror("listen"); close(ss); exit(1);
        }

        printf("Waiting for incoming connections on port %d...\n", port);

        while (1) {
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr); // これあってる？
            s = accept(ss, (struct sockaddr *)&client_addr, &len);// 繋がるまでプログラムはここで待っててくれている！
            if (s < 0) {
                perror("accept"); close(ss); exit(1);
            } else if (s > 0) {
                break;
            }
        }
    }

    // クライアント側の処理
    else if (argc == 3) {
        s = socket(PF_INET, SOCK_STREAM, 0);
        if (s == -1) {
            perror("socket"); exit(1);
        }

        // 接続のための変数用意
        struct sockaddr_in addr; // 構造体定義
        memset(&addr, 0, sizeof(addr)); // メモリ確保
        addr.sin_family = AF_INET; // IPv4
        addr.sin_port = htons(atoi(argv[2])); // ポート
        if (inet_pton(AF_INET, argv[1], &(addr.sin_addr)) <= 0) {  // IPアドレス
            perror("inet_pton"); exit(1);
        };

        // 接続
        if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("connect"); exit(1);
        }
    }

    // 送信の準備
    FILE *recpipe = popen("rec -t raw -b 16 -c 1 -e s -r 44100 -", "r");
    if (recpipe == NULL) {
        perror("popen1"); close(s); exit(1);
    }
    char data[N];
    ssize_t n;

    // 受信の準備
    FILE *playpipe = popen("play -t raw -b 16 -c 1 -e signed -r 44100 -", "w");
    if (playpipe == NULL) {
        perror("popen2"); exit(1);
    }
    char data_2[N];
    ssize_t n_2;

    while (1) {
        n = fread(data, 1, sizeof(data), recpipe);
        if (send(s, data, n, 0) < 0) {
            perror("send"); pclose(recpipe); close(s); exit(1);
        }

        n_2 = recv(s, data_2, N, 0);
        if (n_2 < 0) {
            perror("recv"); exit(1);
        } else if (n_2 == 0) {
            printf("Server closed the connection\n"); exit(1);
        } else {
            fwrite(data_2, sizeof(char), n_2, playpipe);
        }
    }

    pclose(recpipe);
    pclose(playpipe);
    close(s);
    close(ss);
    return 0;
}