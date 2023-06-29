#ifndef PDU_SESSION_EST_ACCEPT_H
#define PDU_SESSION_EST_ACCEPT_H

#include "link_list.h"
#include "mac_info.h"

class PDU_SESSION_ESTABLISHMENT_ACCEPT{
public:
    LINK_LIST<MAC_Info> *member_mac_addre;//发送给UE的其他成员的MAC地址
    int TEID;//GTP-U隧道对端的TEID（UPF侧N3接口的TEID）
};

#endif // PDU_SESSION_EST_ACCEPT_H

