#include<stdio.h>
//网络头文件
#include<winsock2.h>
//库文件
#pragma comment(lib,"WS2_32.lib")
void error_kill(const char* str)
{
	perror(str);

}
//实现网络的初始化
//网络套接字
//port 表示端口 
//如果*port值为0 自动分配一个可用端口
//tinyhttpd服务器 实现server_socket的套接字返回
int startup(unsigned short* port) {
	//todo port 分配
	WSADATA data;
	int ret = WSAStartup(
		MAKEWORD(1, 1),//1.1版本
		&data);
	if (ret != 0)//!ret
	{
		exit(1);
	}

	int server_socket =	//返回值为套接字
		socket(PF_INET,//套接字类型
			SOCK_STREAM,//←数据流 数据报
			IPPROTO_TCP			//指定协议
		);
	if (server_socket == -1)
	{
		//结束提示
		error_kill("socket");
	}

	//端口复用 设置端口可以复用
	int opt = 1;
	ret = setsockopt(server_socket,
		SOL_SOCKET,/* options for socket level */
		SO_REUSEADDR,
		(const char*)&opt, sizeof(opt));
	if (ret == -1)
	{
		error_kill("setsockopt");
	}

	//配置服务器的网络地址
	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));
	
	server_addr.sin_family = PF_INET;//网络地址类型
	server_addr.sin_port = htons(*port);//htons: h本机 to nekwork ushort 类型
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//网络可连接IP设置为any 任意访问 h to network l


	//绑定套接字
;
	if (bind(server_socket,(struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		error_kill("bind");
	}

	//分配动态端口
	int nameLen = sizeof(server_addr);
	if (*port == 0)
	{
		//获取动态端口号
		if (getsockname(server_socket, (struct sockaddr*)&server_addr, &nameLen) < 0)//失败返回值可能为-1
		{
			error_kill("getsockname"); 
		}
		*port = server_addr.sin_port;
	}


	//创建监听队列
	if (listen(server_socket, 5)<0)
	{
		error_kill("error_kill");
	}
	return server_socket;
}
int main(void) {
	unsigned short port = 0;
	int server_sock = startup(&port);
	printf("httpd服务已启动 正在监听%d端口",port);







	return 0;
}