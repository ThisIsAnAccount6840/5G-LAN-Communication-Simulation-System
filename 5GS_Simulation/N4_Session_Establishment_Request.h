#ifndef N4_SESSION_ESTABLISHMENT_REQUEST_H
#define N4_SESSION_ESTABLISHMENT_REQUEST_H

#include "IEs.h"
#include "link_list.h"

#include <QString>

class N4_SESSION_ESTABLISHMENT_REQUEST{
public:
    int node_num;
    bool is_in_multi;
    LINK_LIST<Create_FAR> create_far_all;//2个node，目前只配置local switch相关的规则
    LINK_LIST<Create_PDR> create_pdr_all;//2个node，同上
    N4_SESSION_ESTABLISHMENT_REQUEST(QString = "null", QString ="VN", bool multi=false);
};

#endif // N4_SESSION_ESTABLISHMENT_REQUEST_H


