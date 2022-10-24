// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

// str is a strnig with the LikeServer
// clientLikes are the likes from the above server and totalLikes
// are the total likes the primary likes server has after adding
// the client's logs
void updateLogFile(char str[], int clientLikes, int totalLikes) {
    FILE* fp;
    fp = fopen("/tmp/PrimaryLikesLog", "a");

    // copies str input with LikeServer to new string
    // and concats clientLikes to it
    char clientLine[20];
    strncpy(clientLine, str, 20);


    // concates "Total: " and the total likes
    char totalLine[20] = "Total: ";

    // Convers both int inputs to strings
    char* cstr[4];
    char* tstr[4];
    sprintf(cstr, "%d", clientLikes);
    sprintf(tstr, "%d", totalLikes);

    // Resumes concat from above
    strcat(clientLine, ": ");
    strcat(clientLine, cstr);
    strcat(totalLine, tstr);

    // Adds two new lines to end of log file and closes the file
    fputs(clientLine, fp);
    fprintf(fp, "\n");
    fputs(totalLine, fp);
    fprintf(fp, "\n");
    fclose(fp);
}

int main(int argc, char const* argv[])
{
    int totalLikes = 0;

    // Socket variables used in the connection code
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // Buffer used to read client input
    char buffer[1024] = { 0 };

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Unable to create a socket");
        exit(EXIT_FAILURE);
    }

    // Socket opened on 8080
    if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT, &opt,
        sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address,
        sizeof(address))
        < 0) {
        perror("Unable to Bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 100) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // Continues to read socket until the user exits the program
    // Otherwise it would exit after reading one line.
    while (1) {
        if ((new_socket
            = accept(server_fd, (struct sockaddr*)&address,
                (socklen_t*)&addrlen))
            < 0) {
            perror("Unable to Accept Connection");
            exit(EXIT_FAILURE);
        }

        // Reads the input from the like server and
        // copies it over to new string
        valread = read(new_socket, buffer, 1024);
        char fullMessage[20] = "";
        strncpy(fullMessage, buffer, 20);

        // Splits the original message to find the amount of likes
        // to convert it to an int to add to the total
        char* clientString = strtok(buffer, " ");
        char* splitString = strtok(NULL, " ");
        int clientAmount = atoi(splitString);
        totalLikes = totalLikes + clientAmount;

        // Writes the successful reading to the logfile
        updateLogFile(clientString, clientAmount, totalLikes);

        // Resets the buffer as not to include pervious data in new string from
        // new input
        memset(buffer, 0, 1024);
        printf("Received: %s\n", fullMessage);
        // Sends success back and closes this individual connection
        send(new_socket, fullMessage, strlen(fullMessage), 0);
        close(new_socket);
    }

    return 0;
}