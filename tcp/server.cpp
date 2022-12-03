#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int MAX_BUF_SIZE = 80;

map<string, string> usr_name_password;

void trans(const string &str, char *s) {
    int n = str.size();
    for (int i = 0; i < n; ++i) s[i] = str[i];
    while (n > 0 && (s[n - 1] == '\r' || s[n - 1] == '\n')) --n;
    s[n] = 0;
}

void trans(char *s, string &str) {
    int n = strlen(s);
    str.resize(n);
    for (int i = 0; i < n; ++i) str[i] = s[i];
    while (n > 0 && (str[n - 1] == '\r' || str[n - 1] == '\n')) --n;
    str[n] = 0;
}

void Listen(int serv_sock, int backlog = 20) {
    listen(serv_sock, backlog);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    
    string s, usr_name, password;
    char buf[MAX_BUF_SIZE];
    
    //read(clnt_sock, buf, MAX_BUF_SIZE);
    //trans(buf, s);
    //s = s + " received.";
    //trans(s, buf);
    //puts(buf);
    
    while (true) {    
        s = "Please send your user name: ";
        trans(s, buf);
        write(clnt_sock, buf, sizeof(buf));

        read(clnt_sock, buf, MAX_BUF_SIZE);
        trans(buf, usr_name);

        s = "Please send your password: ";
        trans(s, buf);
        write(clnt_sock, buf, sizeof(buf));

        read(clnt_sock, buf, MAX_BUF_SIZE);
        trans(buf, password);

        if (!usr_name_password.count(usr_name) || 
                usr_name_password[usr_name] != password) {
            s = "Invalid user name or wrong password, please try it again.";
            trans(s, buf);
            write(clnt_sock, buf, sizeof(buf));
        } else {
            s = "Correct! You are entered.";
            trans(s, buf);
            write(clnt_sock, buf, sizeof(buf));
            cout << "The user " << usr_name << " is entered.\n";
        }
        read(clnt_sock, buf, MAX_BUF_SIZE);
        trans(buf, s);
        if (s == "no") {
            continue;
        } else {
            break;
        }
    }

    close(clnt_sock);
}

int main(){
    puts("The server is now on.");

    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(3344);
        

    usr_name_password["iNx"] = "123456";
    
    while (true) {
        int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        
        Listen(serv_sock);
        
        close(serv_sock);
        
        puts("Do you want to close this server? yes / no : (y / n)");
        string s;
        cin >> s;
        if (s[0] == 'y' || s[0] == 'Y') {
            break;
        }        
        
    }
    return 0;
}
