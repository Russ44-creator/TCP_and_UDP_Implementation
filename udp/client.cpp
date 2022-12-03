#include <bits/stdc++.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/*
'0': hello, and send usr_name
'1': send password
'2': feedback
*/

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
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    serv_addr.sin_port = htons(2333); 
   
    string s;
    char buf[MAX_BUF_SIZE];
    
    s = "hello!0";
    trans(s, buf);
    sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while (true) {  
        struct sockaddr_in p_addr;
        socklen_t p_addr_size = sizeof(p_addr);
        memset(&p_addr, 0, sizeof(p_addr));
        int len = recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr*)&p_addr, &p_addr_size);
        
        len = min(len, (int)strlen(buf));
        
        if (len < 0) {
            puts("ERROR");
            break;
        } else if (len == 0) {
            continue;
        }
        
        if (!(p_addr.sin_addr.s_addr == serv_addr.sin_addr.s_addr && 
            p_addr.sin_port == serv_addr.sin_port)) continue;
            
        char sign = buf[--len];
        buf[len] = 0;
        
        if (sign == '0') {
            puts(buf);
            cin >> s;
            s = s + "1";
        } else if (sign == '1') {
            puts(buf);
            cin >> s;
            s = s + "2";
        } else if (sign == '2'){
            puts(buf);
            puts("press '0' to say hello again, and exit otherwise.");
            cin >> s;
            if (s[0] == '0') {
                s = "hello!0";
            } else break;
        } else {
            continue;   
        }
                
        trans(s, buf);
        sendto(sock, buf, sizeof(buf), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        
        puts("Do you want to close this client? yes / no : (y / n)");
        cin >> s;
        if (s[0] == 'y' || s[0] == 'Y') break;
    }
   
    close(sock);

    return 0;
}
