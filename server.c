#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>

int main() {
	const int PORT = 8080;

	int sock = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port = htons(PORT),
		.sin_addr.s_addr = INADDR_ANY
	};

	if (sock < 0) {
		perror("Socket");
		return -1;
	}

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return -1;
	}

	if (listen(sock, 1) < 0) {
		perror("Listen");
		return -1;
	}

	while (1) {
		struct sockaddr client_addr;
		socklen_t client_len = sizeof(client_addr);

		int client_fd = accept(sock, (struct sockaddr *)&client_addr, &client_len);

		if (client_fd < 0) {
			perror("Accept");
			continue;
		}

		char buffer[1024] = {0};
		ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
		if (bytes_read > 0) {
			printf("Received request:\n%s\n", buffer);
		}

		const char *response = "Request received\n";
		write(client_fd, response, strlen(response));

		close(client_fd);
	}

	close(sock);
	return 0;
}
