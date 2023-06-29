#ifndef REGISTRATION_REQUEST_H
#define REGISTRATION_REQUEST_H

#include <QString>
#include "link_list.h"

class REGISTRATION_REQUEST
{
public:
    REGISTRATION_REQUEST(QString, bool, LINK_LIST<QString>, QString );
    ~REGISTRATION_REQUEST();
    QString supi;
    bool LAN_Capability;
    LINK_LIST<QString> Group_Member_ID;
    QString MAC_addr;
    //QString IP_addr;
};

#endif // REGISTRATION_REQUEST_H
