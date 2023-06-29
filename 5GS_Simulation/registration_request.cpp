#include "registration_request.h"

REGISTRATION_REQUEST::REGISTRATION_REQUEST(QString s, bool LAN, LINK_LIST<QString> id, QString mac)
{
    supi=s;
    LAN_Capability=LAN;
    Group_Member_ID=id;
    MAC_addr=mac;
}

REGISTRATION_REQUEST::~REGISTRATION_REQUEST(){

}
