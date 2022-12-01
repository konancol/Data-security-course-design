#include"../tcpSocket/tcpSocket.h"
#include"../tcpSocket/flieOperation.h"
#include"../aes&ras/myaes.h"

int main()
{
	unsigned char pt[17];
	unsigned char expansionkey[15 * 16];
	int key[8] = {2,0,0,2,1,0,1,1};

	init_Socket();

	SOCKET fd = create_clientSocket("127.0.0.1");
	printf("coonet server successed..\n");

//	printf("Please input key:\n");
//	scanf("%s", key);
	printf("The key is:");
	for (int i = 0; i < 8; i++)
	{
		printf("%d ", key[i]);
	}
	printf("\n");
//	send(fd, key, strlen(key), 0);

	char recvBuff[1024];
	while (1) {
		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		printf("Please input:\n");
		scanf("%s", sendBuff);

		ScheduleKey(key, expansionkey, 4, 10);	//1、密钥扩展生成
		AesEncrypt(sendBuff, expansionkey, 10);		//2、AES 加密
	//	memcpy(sendBuff, pt, sizeof(sendBuff));
		send(fd, sendBuff, strlen(sendBuff), 0);

		memset(recvBuff, 0, sizeof(recvBuff));
		recv(fd, recvBuff, sizeof(recvBuff) - 1, 0);
		Contrary_AesEncrypt(recvBuff, expansionkey, 10);//AES 解密
		printf("receive message: %s\n", recvBuff);
	}

/*	char recvBuf[1024] = "";
	int ret = recv(fd, recvBuf, 1024, 0);
	if (ret == 0)
	{
		printf("服务器正常下线。。。\n");
	}
	else if (ret < 0)
	{
		err("recv");
	}
	puts(recvBuf);
*/

/*	char sendBuf[1024] = "nihao";
	send(fd, sendBuf, strlen(sendBuf), 0);
*/
	closesocket(fd);
	close_Socket();
	getchar();


	return 0;
}