#ifndef LAN_INFO_H
#define LAN_INFO_H

#include <QString>
#include "link_list.h"

class LAN_INFO //只封装一个LAN组的信息
{
public:
    LAN_INFO();
    LAN_INFO(QString, QString, int, int, LINK_LIST<QString> &);
    QString LAN_Group_Name;
    QString DNN;
    int SST;
    int SD;
    LINK_LIST<QString> LAN_Member_ID;//该LAN组内所有成员的ID
};

#endif // LAN_INFO_H
