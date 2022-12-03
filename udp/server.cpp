#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
'0': hello
'1': usr_name
'2': password
*/


using namespace std;

const int MAX_BUF_SIZE = 80;

map<string, string> usr_name_password, dialog;

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
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    
    string s, usr_name, password;
    char buf[MAX_BUF_SIZE];
    
    while (true) {
        memset(&clnt_addr, 0, sizeof(clnt_addr));
        
        int len = recvfrom(serv_sock, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        
        len = min(len, (int)strlen(buf));
        
        if (len < 0) {
            puts("ERROR!");
            break;
        } else if (len == 0) {
            continue;
        }   
        
        char sign = buf[--len];
        buf[len] = 0;
        
        if (sign == '0') {
            s = "Hello! Please send your user name: 0";
        } else if (sign == '1') {
            trans(buf, usr_name);
            s = "Please send your password: 1";  
        } else if (sign == '2') {
            trans(buf, password);
            if (!usr_name_password.count(usr_name) ||
                usr_name_password[usr_name] != password) {
                s = "Invalid user name or wrong password, please try it again. 2";    
            } else {
                s = "Correct! You are entered. 2";
                cout << "The user " << usr_name << " is entered.\n";
            }
        } else {
            continue;
        }
        
        trans(s, buf);
        sendto(serv_sock, buf, sizeof(buf), 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
        
        //puts("Do you want to close this server? yes / no : (y / n)");
        //string s;
        //cin >> s;
        //if (s[0] == 'y' || s[0] == 'Y') break;
    }
}

int main(){
    puts("The server is now on.");

    int serv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(2333);
        
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    usr_name_password["iNx"] = "123456";
    
    Listen(serv_sock);
    
    close(serv_sock);
    return 0;
}
