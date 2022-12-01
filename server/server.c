#include"../tcpSocket/tcpSocket.h"
#include"../tcpSocket/flieOperation.h"
#include"../aes&ras/myaes.h"


int main()
{
	unsigned char pt[17];
	int key[8] = {2,0,0,2,1,0,1,1};
	unsigned char expansionkey[15 * 16];

	init_Socket();

	SOCKET serfd = create_serverSocket();
	printf("server create successed ,wait client connet...\n");
	//等待客户端连接
	SOCKET clifd = accept(serfd, NULL, NULL);
	if (clifd == INVALID_SOCKET)
	{
		err("accept");
	}printf("success\n");
	//可以和客户端进行通信
//	recv(clifd, key, strlen(key), 0);
	printf("The key is:");
	for (int i = 0; i < 8; i++)
	{
		printf("%d ", key[i]);
	}
	printf("\n");
/*
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);
	SOCKET clientSocket = accept(serfd, (SOCKADDR*)&clientAddr, &len);
	*/
	char recvBuff[1024];
	while (1) {
		memset(recvBuff, 0, sizeof(recvBuff));
		ScheduleKey(key, expansionkey, 4, 10);	//1、密钥扩展生成
		recv(clifd, recvBuff, sizeof(recvBuff) - 1, 0);
		Contrary_AesEncrypt(recvBuff, expansionkey, 10);//AES 解密
		printf("receive message: %s\n", recvBuff);

		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		printf("Please input:\n");
		scanf("%s", sendBuff);
		
		AesEncrypt(sendBuff, expansionkey, 10);		//2、AES 加密
		send(clifd,sendBuff,strlen(sendBuff),0);
	}

/*	if (SOCKET_ERROR == send(clifd, "hello", 5, 0))
	{
		err("hello send");
	}

	char recvBuf[1024] = "";
	int ret = recv(clifd, recvBuf, 1024, 0);
	if (ret == 0)
	{
		printf("客户端正常下线。。。\n");
	}
	else if (ret < 0)
	{
		err("recv");
	}
	puts(recvBuf);

*/
	closesocket(clifd);
	closesocket(serfd);
	
	close_Socket();

	getchar();

	return 0;
}