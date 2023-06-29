#ifndef MAC_INFO_H
#define MAC_INFO_H

#include "link_list.h"
#include <QDebug>

class MAC_Info //存储一个VN组内某个成员的SUPI和对应的MemberID以及IP地址
{
public:
    MAC_Info();
    QString SUPI;
    QString Member_ID;
    QString MAC_Addr;
    void print_mac_info();
};

class other_ue_info{ //存储同一个组的其他UE的信息和该组内通信时应使用的PDU session ID
public:
    QString PDUSession_ID;
    LINK_LIST<MAC_Info> other_member_mac_info_list;
    void print_other_ue_info();
};

#endif // MAC_INFO_H
