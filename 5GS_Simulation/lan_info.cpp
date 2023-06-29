#include "lan_info.h"

LAN_INFO::LAN_INFO(){
    LAN_Group_Name="NULL";
    DNN="NULL";
    SST=-1;
    SD=-1;
}

LAN_INFO::LAN_INFO(QString group, QString dnn, int sst, int sd, LINK_LIST<QString> &ids)
{
    LAN_Group_Name=group;
    DNN=dnn;
    SST=sst;
    SD=sd;
    LAN_Member_ID=ids;
}
