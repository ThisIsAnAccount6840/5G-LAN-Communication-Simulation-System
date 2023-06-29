#ifndef UE_INFO_H
#define UE_INFO_H

#include <QString>

class UE_Info
{
public:
    UE_Info();
    QString supi;
    bool LAN_Capability;
    QString Group_Member_ID;
    QString MAC_addr;
};

#endif // UE_INFO_H
