/*
muduo�������û��ṩ��������Ҫ����
TcpServer:���ڱ�д�����������
TcpClient:���ڱ�д�ͻ��˳���

��epoll+�̳߳ط�װ����
�ô����ܹ�������io�Ĵ����ҵ��������ֿ�
                         �û������ӺͶϿ�    �û��Ŀɶ�д�¼�

*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

/*����muduo����⿪������������
1.���TcpServer����
2.����EventLoop�¼�ѭ�������ָ��
3.��ȷTcpServer���캯����Ҫʲô����
4.�ڵ�ǰ��������Ĺ��캯�����У�ע�ᴦ�����ӵĻص������ʹ����д�¼��Ļص�����
5.���ú��ʵķ������߳�������muduo����Լ�����I/O�̺߳�worker�߳�
*/
class ChatServer
{
public:
    ChatServer(EventLoop* loop,//�¼�ѭ��
            const InetAddress& listenAddr,//IP+Port
            const string& nameArg)//������������
        : _server(loop,listenAddr,nameArg), _loop(loop)
    {
        //��������ע���û����ӵĴ����ͶϿ��ص�
        _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,_1));
        //��������ע���û���д�¼��ص�
        _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,_1,_2,_3));
        
        //���÷������˵��߳�����  1��I/O�߳� 3��worker�߳�
        _server.setThreadNum(4);
    }

    //�����¼�ѭ��
    void start(){
        _server.start();
    }
private:
    //ר�Ŵ����û������Ӵ����ͶϿ� epoll listenfd accept
    void onConnection(const TcpConnectionPtr &conn){
        if(conn->connected()){
            cout<<conn->peerAddress().toIpPort()<<"->"<<
            conn->localAddress().toIpPort()<<
            "state:online"<<endl;
        }else{
            cout<<conn->peerAddress().toIpPort()<<"->"<<
            conn->localAddress().toIpPort()<<
            "state:offline"<<endl;
            conn->shutdown();//close(fd)
        
        }
        
    }
    void onMessage(const TcpConnectionPtr &conn,//����
                    Buffer *buffer,//������
                    Timestamp time)//���յ����ݵ�ʱ����Ϣ
    {
        string buf=buffer->retrieveAllAsString();
        cout<<"recv data:"<<buf<<"time:"<<time.toString()<<endl;
        conn->send(buf);
    }
    TcpServer _server;//1.����һ��server�������ǵ�һ��
    EventLoop *_loop;
};

int main()
{
    EventLoop loop;//epoll,�ر��񴴽���epoll
    InetAddress addr("127.0.0.1",6000);
    ChatServer server(&loop,addr,"ChatServer");

    server.start();//�����˷���listenfd epoll_ctl��ӵ�epoll��
    loop.loop();//�൱�ڵ�����epoll_wait��������ʽ�ȴ����û����ӣ��������û��Ķ�д�¼���

    return 0;
}
