#ifndef __FILEOP_H_
#define __FILEOP_H_
#include"tcpSocket.h"

/********服务器*******/
//读取文件
bool readFile(const char* filename);
//发送文件
bool sendFile(SOCKET s, const char* fileName);


/********客户端*******/
//接收文件
bool recvFile(SOCKET s, const char* fileName);
//保存文件
bool saveFile(const char* filename);



#endif // !__FILEOP_H_
