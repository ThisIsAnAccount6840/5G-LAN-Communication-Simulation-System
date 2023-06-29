#ifndef UE_SUB_DATA_H
#define UE_SUB_DATA_H

#include <QString>
#include "link_list.h"

class UE_Sub_Data{
public:
    UE_Sub_Data();
    ~UE_Sub_Data();
    QString SUPI;
    //LAN Info
    bool LAN_CAP;
    int LAN_num;//UE能访问的LAN组数量
    LINK_LIST<QString> LAN_Group_Name;//All the LAN groups that this UE can access
    LINK_LIST<QString> LAN_Member_ID;//UE在不同LAN组中的member ID
    LINK_LIST<QString> DNN_list;//上述LAN组对应的DNN
    LINK_LIST<int> SST;
    LINK_LIST<int> SD;

    QString* find_Group_Name(QString *ID);
};

#endif // UE_SUB_DATA_H
