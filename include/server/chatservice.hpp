#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <unordered_map>
#include <functional>
#include <muduo/net/TcpConnection.h>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "redis.hpp"
#include "json.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"
using json=nlohmann::json;

//������Ϣ���¼��ص�����
using MsgHandler =std::function<void(const TcpConnectionPtr &conn,json &js,Timestamp)>;

//���������ҵ����
class ChatService
{
public:
    //��ȡ��������Ľӿں���
    static ChatService* instance();
    //�����¼ҵ��
    void login(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //����ע��ҵ��
    void reg(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //һ��һ����ҵ��
    void oneChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //��Ӻ���ҵ��
    void addFriend(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //�������쳣��ҵ�����÷���
    void reset();
     // ����ע��ҵ��
    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);
    //����ͻ����쳣�˳�
    void clientCloseException(const TcpConnectionPtr &conn);
    //����Ⱥ��ҵ��
    void createGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //����Ⱥ��ҵ��
    void addGroup(const TcpConnectionPtr &conn,json &js,Timestamp time);
    //Ⱥ������ҵ��
    void groupChat(const TcpConnectionPtr &conn,json &js,Timestamp time);
    // ��ȡ��Ϣ��Ӧ�Ĵ�����
    MsgHandler getHandler(int msgid);
    // ��redis��Ϣ�����л�ȡ���ĵ���Ϣ
    void handleRedisSubscribeMessage(int, string);
private:
    ChatService();

    //�洢��Ϣid�����Ӧ��ҵ������
    unordered_map<int,MsgHandler> _msgHandlerMap;

    //�洢�����û���ͨ������
    unordered_map<int,TcpConnectionPtr> _userConnMap;

    //���廥��������֤_userConnMap���̰߳�ȫ
    mutex _connMutex;

    //���ݲ��������
    //�ṩ����������ֶΣ������
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;

    // redis��������
    Redis _redis;
};



#endif