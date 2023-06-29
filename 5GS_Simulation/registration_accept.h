#ifndef REGISTRATION_ACCEPT_H
#define REGISTRATION_ACCEPT_H

#include <QString>

#include "lan_info.h"
#include "link_list.h"

class REGISTRATION_ACCEPT
{
public:
    REGISTRATION_ACCEPT(LAN_INFO **);
    LAN_INFO* lan_config[2];

};

#endif // REGISTRATION_ACCEPT_H
