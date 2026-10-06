#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
	const int PORT = 8080;
	char ip[] = "127.0.0.1";

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT)
	};

	if (sock < 0) {
		perror("Socket");
		return -1;
	}

	if(inet_pton(AF_INET, ip, &addr.sin_addr) < 0) {
		perror("inet_pton");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Connect");
		return -1;
	}

	write(sock, "Hello", 5);

	char buffer[1024] = {0};
	ssize_t bytes_read = read(sock, buffer, sizeof(buffer) - 1);
	if (bytes_read > 0) {
		printf("Response from server: %s\n", buffer);
	}

	close(sock);
	return 0;
}
