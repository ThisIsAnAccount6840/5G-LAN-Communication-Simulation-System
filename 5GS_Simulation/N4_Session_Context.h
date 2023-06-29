#ifndef N4_SESSION_CONTEXT_H
#define N4_SESSION_CONTEXT_H

#include "IEs.h"
#include "link_list.h"
#include <QString>

class N4_SESSION_CONTEXT{
public:
    QString UE_IP;//该N4会话对应的UE的IP地址
    LINK_LIST<Create_PDR> PDR_LIST;
    LINK_LIST<Create_FAR> FAR_LIST;
    N4_SESSION_CONTEXT();
    Create_PDR* find_matching_pdr_in_N4SessionContext(PDI &);
    NODE<Create_PDR>* find_PDR_with_ID(int);
    NODE<Create_FAR>* find_FAR_with_ID(int);

    void print_N4SessionContext();
};

#endif // N4_SESSION_CONTEXT_H
