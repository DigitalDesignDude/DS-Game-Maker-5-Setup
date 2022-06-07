#ifndef _PA_WIFI
#define _PA_WIFI


#include <dswifi9.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define IPADDR(d,c,b,a) ((unsigned long)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((c)&0xff)<<8)|(((d)&0xff))))

#define PA_NORMAL_TCP 1
#define PA_NONBLOCKING_TCP 2

#define PA_InitWifi() Wifi_InitDefault(INIT_ONLY)

bool PA_ConnectWifiWFC();
unsigned long PA_chartoip(char *message);
int PA_InitSocket(int* socket, char* host, int port, int mode);
int PA_InitServer(int* sock, int port, int mode, int num_connect);

bool PA_SubmitScore(char *response, char *gamekey, char *playername, int score, char *level, u8 responsetype);

#endif
