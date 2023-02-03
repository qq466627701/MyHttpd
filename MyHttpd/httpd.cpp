#include<stdio.h>
//����ͷ�ļ�
#include<winsock2.h>
//���ļ�
#pragma comment(lib,"WS2_32.lib")
void error_kill(const char* str)
{
	perror(str);

}
//ʵ������ĳ�ʼ��
//�����׽���
//port ��ʾ�˿� 
//���*portֵΪ0 �Զ�����һ�����ö˿�
//tinyhttpd������ ʵ��server_socket���׽��ַ���
int startup(unsigned short* port) {
	//todo port ����
	WSADATA data;
	int ret = WSAStartup(
		MAKEWORD(1, 1),//1.1�汾
		&data);
	if (ret != 0)//!ret
	{
		exit(1);
	}

	int server_socket =	//����ֵΪ�׽���
		socket(PF_INET,//�׽�������
			SOCK_STREAM,//�������� ���ݱ�
			IPPROTO_TCP			//ָ��Э��
		);
	if (server_socket == -1)
	{
		//������ʾ
		error_kill("socket");
	}

	//�˿ڸ��� ���ö˿ڿ��Ը���
	int opt = 1;
	ret = setsockopt(server_socket,
		SOL_SOCKET,/* options for socket level */
		SO_REUSEADDR,
		(const char*)&opt, sizeof(opt));
	if (ret == -1)
	{
		error_kill("setsockopt");
	}

	//���÷������������ַ
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = PF_INET;//�����ַ����
	server_addr.sin_port = htons(*port);//htons: h���� to nekwork ushort ����
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//���������IP����Ϊany ������� h to network l


	//���׽���
;
	if (bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		error_kill("bind");
	}

	//���䶯̬�˿�
	int nameLen = sizeof(server_addr);
	if (*port == 0)
	{
		//��ȡ��̬�˿ں�
		if (getsockname(server_socket, (struct sockaddr*)&server_addr, &nameLen) < 0)//ʧ�ܷ���ֵ����Ϊ-1
		{
			error_kill("getsockname"); 
		}
		*port = server_addr.sin_port;
	}


	//������������
	if (listen(server_socket, 5)<0)
	{
		error_kill("error_kill");
	}
	return server_socket;
}
int main(void) {
	unsigned short port = 0;
	int server_sock = startup(&port);
	printf("httpd���������� ���ڼ���%d�˿�",port);







	return 0;
}