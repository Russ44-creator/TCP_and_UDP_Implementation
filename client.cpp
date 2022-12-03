#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

const int MAX_BUF_SIZE = 80;

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
    while (n > 0 && (s[n - 1] == '\r' || s[n - 1] == '\n')) --n;
    str[n] = 0;
}

int main(){
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(3344); 
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
   
    string s;
    char buf[MAX_BUF_SIZE];
    
    //write(sock, buf, sizeof(buf));

    while (true) {
        read(sock, buf, MAX_BUF_SIZE);
        puts(buf);
         
        cin >> s;
        trans(s, buf);
        write(sock, buf, sizeof(buf));
        
        read(sock, buf, MAX_BUF_SIZE);
        puts(buf);
         
        cin >> s;
        trans(s, buf);
        write(sock, buf, sizeof(buf));
        
        read(sock, buf, MAX_BUF_SIZE);
        puts(buf);
        
        puts("Do you want to end this connect? yes / no : (y / n):");
        cin >> s;
        if (s[0] == 'y' || s[0] == 'Y') {
            s = "yes";
        } else {
            s = "no";
        }
        
        trans(s, buf);
        write(sock, buf, sizeof(buf));
        
        if (s == "yes") break;
    }
   
    close(sock);

    return 0;
}
