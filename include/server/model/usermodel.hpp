#ifndef USERMODEL_H
#define USERMODEL_H

#include "user.hpp"

//User表的数据操作类，而user.hpp只是定义字段的，而这个是针对表的增删改查
class UserModel {
public:
    //User表的增加方法
    bool insert(User &user);

    //根据用户号码查询用户信息
    User query(int id);

    //更新用户的状态信息
    bool updateState(User user);

    //重置用户的状态信息
    void resetState();
};
#endif