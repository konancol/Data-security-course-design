#ifndef __FILEOP_H_
#define __FILEOP_H_
#include"tcpSocket.h"

/********������*******/
//��ȡ�ļ�
bool readFile(const char* filename);
//�����ļ�
bool sendFile(SOCKET s, const char* fileName);


/********�ͻ���*******/
//�����ļ�
bool recvFile(SOCKET s, const char* fileName);
//�����ļ�
bool saveFile(const char* filename);



#endif // !__FILEOP_H_
