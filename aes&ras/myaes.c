
#include "myaes.h"

unsigned char x2time(unsigned char x)
{
	if (x & 0x80)
	{
		return (((x << 1) ^ 0x1B) & 0xFF);
	}
	return x << 1;
}

unsigned char x3time(unsigned char x)
{
	return (x2time(x) ^ x);
}

unsigned char x4time(unsigned char x)
{
	return (x2time(x2time(x)));
}

unsigned char x8time(unsigned char x)
{
	return (x2time(x2time(x2time(x))));
}

unsigned char x9time(unsigned char x)
{
	return (x8time(x) ^ x);
}

unsigned char xBtime(unsigned char x)
{
	return (x8time(x) ^ x2time(x) ^ x);
}

unsigned char xDtime(unsigned char x)
{
	return (x8time(x) ^ x4time(x) ^ x);
}

unsigned char xEtime(unsigned char x)
{
	return (x8time(x) ^ x4time(x) ^ x2time(x));
}

void MixColumns(unsigned char* col)
{
	unsigned char tmp[4], xt[4];
	int i;
	for (i = 0; i < 4; i++, col += 4)  //col代表一列的基地址，col+4:下一列的基地址
	{
		/*
		xt[0]=x2time(col[0]);
		xt[1]=x2time(col[1]);
		xt[2]=x2time(col[2]);
		xt[3]=x2time(col[3]);
		//xt[n]代表*2   xt[n]^col[n]代表*3   col[n]代表*1
		tmp[0]=(xt[0])^(xt[1]^col[1])^col[2]^col[3];	//2 3 1 1
		tmp[1]=col[0]^(xt[1])^(xt[2]^col[2])^col[3];	//1 2 3 1
		tmp[2]=col[0]^col[1]^(xt[2])^(xt[3]^col[3]);	//1 1 2 3
		tmp[3]=(xt[0]^col[0])^col[1]^col[2]^(xt[3]);	//3 1 1 2
		*/
		tmp[0] = x2time(col[0]) ^ x3time(col[1]) ^ col[2] ^ col[3];	//2 3 1 1
		tmp[1] = col[0] ^ x2time(col[1]) ^ x3time(col[2]) ^ col[3];	//1 2 3 1
		tmp[2] = col[0] ^ col[1] ^ x2time(col[2]) ^ x3time(col[3]);	//1 1 2 3
		tmp[3] = x3time(col[0]) ^ col[1] ^ col[2] ^ x2time(col[3]);	//3 1 1 2
		//修改后的值 直接在原矩阵上修改
		col[0] = tmp[0];
		col[1] = tmp[1];
		col[2] = tmp[2];
		col[3] = tmp[3];
	}
}

void Contrary_MixColumns(unsigned char* col)
{
	unsigned char tmp[4];
	unsigned char xt2[4];//colx2
	unsigned char xt4[4];//colx4
	unsigned char xt8[4];//colx8
	int x;
	for (x = 0; x < 4; x++, col += 4)
	{
		/*
		xt2[0]=x2time(col[0]);
		xt2[1]=x2time(col[1]);
		xt2[2]=x2time(col[2]);
		xt2[3]=x2time(col[3]);
		xt4[0]=x2time(xt2[0]);
		xt4[1]=x2time(xt2[1]);
		xt4[2]=x2time(xt2[2]);
		xt4[3]=x2time(xt2[3]);
		xt8[0]=x2time(xt4[0]);
		xt8[1]=x2time(xt4[1]);
		xt8[2]=x2time(xt4[2]);
		xt8[3]=x2time(xt4[3]);
		tmp[0]=xt8[0]^xt4[0]^xt2[0]^xt8[1]^xt2[1]^col[1]^xt8[2]^xt4[2]^col[2]^xt8[3]^col[3];
		tmp[1]=xt8[0]^col[0]^xt8[1]^xt4[1]^xt2[1]^xt8[2]^xt2[2]^col[2]^xt8[3]^xt4[3]^col[3];
		tmp[2]=xt8[0]^xt4[0]^col[0]^xt8[1]^col[1]^xt8[2]^xt4[2]^xt2[2]^xt8[3]^xt2[3]^col[3];
		tmp[3]=xt8[0]^xt2[0]^col[0]^xt8[1]^xt4[1]^col[1]^xt8[2]^col[2]^xt8[3]^xt4[3]^xt2[3];
		*/
		tmp[0] = xEtime(col[0]) ^ xBtime(col[1]) ^ xDtime(col[2]) ^ x9time(col[3]);
		tmp[1] = x9time(col[0]) ^ xEtime(col[1]) ^ xBtime(col[2]) ^ xDtime(col[3]);
		tmp[2] = xDtime(col[0]) ^ x9time(col[1]) ^ xEtime(col[2]) ^ xBtime(col[3]);
		tmp[3] = xBtime(col[0]) ^ xDtime(col[1]) ^ x9time(col[2]) ^ xEtime(col[3]);
		col[0] = tmp[0];
		col[1] = tmp[1];
		col[2] = tmp[2];
		col[3] = tmp[3];
	}
}

void ShiftRows(unsigned char* col)
{
	/*
		1 5  9 13				5  9 13 1
		2 6 10 14				10 14 2 6
		3 7 11 15				15 3 7 11
		4 8 12 16				16 4 8 12
	*/
	unsigned char t;
	/*1nd row*///左移1位
	t = col[1]; col[1] = col[5]; col[5] = col[9]; col[9] = col[13]; col[13] = t;
	/*2rd row*///左移2位，交换2次数字来实现
	t = col[2]; col[2] = col[10]; col[10] = t;
	t = col[6]; col[6] = col[14]; col[14] = t;
	/*3th row*///左移3位，相当于右移1次
	t = col[15]; col[15] = col[11]; col[11] = col[7]; col[7] = col[3]; col[3] = t;
	/*4th row*/	//第4行不移位
}

void Contrary_ShiftRows(unsigned char* col)
{
	unsigned char t;
	/*1nd row*/
	t = col[13]; col[13] = col[9]; col[9] = col[5]; col[5] = col[1]; col[1] = t;
	/*2rd row*/
	t = col[2]; col[2] = col[10]; col[10] = t;
	t = col[6]; col[6] = col[14]; col[14] = t;
	/*3th row*/
	t = col[3]; col[3] = col[7]; col[7] = col[11]; col[11] = col[15]; col[15] = t;
	/*4th row*/	//第4行不移位
}

void SubBytes(unsigned char* col)
{
	int x;
	for (x = 0; x < 16; x++)
	{
		col[x] = sbox[col[x]];
	}
}

void Contrary_SubBytes(unsigned char* col)
{
	int x;
	for (x = 0; x < 16; x++)
	{
		col[x] = contrary_sbox[col[x]];
	}
}

void AddRoundKey(unsigned char* col, unsigned char* expansionkey, int round)
{
	//扩展密钥：44*32bit =11*4* 4*8 =  16字节*11轮，每轮用16字节密钥
	//第0轮，只进行一次轮密钥加
	//第1-10轮，轮密钥加
	int x;
	for (x = 0; x < 16; x++)	//每1轮操作：4*32bit密钥 = 16个字节密钥
	{
		col[x] ^= expansionkey[(round << 4) + x];
	}
}

void AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr)
{
	//输入blk原文，直接在上面修改，输出blk密文
	//输入skey：
	//输入Nr = 10轮
	int round;
	//第1轮之前：轮密钥加
	AddRoundKey(blk, expansionkey, 0);
	//第1-9轮：4类操作：字节代换、行移位、列混合、轮密钥加
	for (round = 1; round <= (Nr - 1); round++)
	{
		SubBytes(blk);		//输入16字节数组，直接在原数组上修改
		ShiftRows(blk);		//输入16字节数组，直接在原数组上修改
		MixColumns(blk);	//输入16字节数组，直接在原数组上修改
		AddRoundKey(blk, expansionkey, round);
	}
	//第10轮：不进行列混合
	SubBytes(blk);
	ShiftRows(blk);
	AddRoundKey(blk, expansionkey, Nr);
}

void Contrary_AesEncrypt(unsigned char* blk, unsigned char* expansionkey, int Nr)
{
	int x;
	/* unsigned char *contrary_key=key;
	for(x=0;x<11;x++,key+=16)
	Contrary_MixColumns(key);*/
	AddRoundKey(blk, expansionkey, Nr);
	Contrary_ShiftRows(blk);
	Contrary_SubBytes(blk);
	for (x = (Nr - 1); x >= 1; x--)
	{
		AddRoundKey(blk, expansionkey, x);
		Contrary_MixColumns(blk);
		Contrary_ShiftRows(blk);
		Contrary_SubBytes(blk);
	}
	AddRoundKey(blk, expansionkey, 0);
}

void ScheduleKey(unsigned char* inkey, unsigned char* outkey, int Nk, int Nr)
{
	//inkey:初始16字节密钥key
	//outkey：11组*16字节扩展密钥expansionkey
	//Nk：4列
	//Nr：10轮round
	unsigned char temp[4], t;
	int x, i;
	/*copy the key*/
	//第0组：[0-3]直接拷贝
	for (i = 0; i < (4 * Nk); i++)
	{
		outkey[i] = inkey[i];
	}
	//第1-10组：[4-43]
	i = Nk;
	while (i < (4 * (Nr + 1))) //i=4~43 WORD 32bit的首字节地址，每一个4字节
	{//1次循环生成1个字节扩展密钥，4次循环生成一个WORD
		//temp：4字节数组：代表一个WORD密钥
		/*temp=w[i-1]*/
		//i不是4的倍数的时候
		//每个temp = 每个outkey32bit = 4字节
		for (x = 0; x < 4; x++)
			temp[x] = outkey[(4 * (i - 1)) + x];	//i：32bit的首字节地址
		//i是4的倍数的时候
		if (i % Nk == 0)
		{
			/*字循环：循环左移1字节 RotWord()*/
			t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
			/*字节代换：SubWord()*/
			for (x = 0; x < 4; x++)
			{
				temp[x] = sbox[temp[x]];
			}
			/*轮常量异或：Rcon[j]*/
			temp[0] ^= Rcon[(i / Nk) - 1];
		}
		//else if(Nk>6 && (i%Nk)==4)	//Nk>6的算法不同，暂时用不到
		//{
		//	/*SubWord*/
		//	for(x=0;x<4;x++)
		//	{
		//		temp[x]=sbox[temp[x]];
		//	}
		//}

		/*w[i] = w[i-4]^w[i-1]*/
		for (x = 0; x < 4; x++)
		{
			outkey[(4 * i) + x] = outkey[(4 * (i - Nk)) + x] ^ temp[x];
		}
		++i;
	}
}


//
//int main(void) {
//	/*
//	pt：原文16字节-->密文
//	key：原密钥16字节
//	skey：密钥扩展44long
//	sbox：s盒
//	*/
//	unsigned char pt[17], key[17];
//	unsigned char expansionkey[15 * 16];
//	int i;
//	int j;
//	
//	printf("please input plaintext\n");//输入无格式的字符串字符个数不得少于六个！！！！尽量用不同的数字或字母
//	scanf("%s", pt);
//	printf("please input key\n");//输入加密钥匙密匙个数不得低于六个！！！！尽量用不同的数字或字母
//	scanf("%s", key);
//
//	/*加密*/
//	ScheduleKey(key, expansionkey, 4, 10);	//1、密钥扩展生成
//	AesEncrypt(pt, expansionkey, 10);		//2、AES 加密
//	printf("AesEncrypt text is: ");	//输出密码文件
//	for (i = 0; i < 16; i++)
//	{
//		printf("%02x ", pt[i]);
//	}
//	printf("\n");
//	printf("\n");
//
//	/*解密*/
//	Contrary_AesEncrypt(pt, expansionkey, 10);//AES 解密
//	printf("after Contrary_AesEncrypt,plaintext is: ");//将解密文件输出
//	for (i = 0; i < 16; i++)
//	{
//		printf("%c ", pt[i]);
//	}
//	printf("\n");
//	printf("\n");
//	while (1);
//	return 0;
//}