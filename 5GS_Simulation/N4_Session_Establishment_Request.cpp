#include "N4_Session_Establishment_Request.h"

N4_SESSION_ESTABLISHMENT_REQUEST::N4_SESSION_ESTABLISHMENT_REQUEST(QString ue_ip, QString nw_ins, bool multi){
    node_num=2;
    is_in_multi=multi;
    int i;
    NODE<Create_FAR> *f;
    NODE<Create_PDR> *p;
    for(i=0;i<node_num;i++){
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=20+i;
        create_far_all.Insert_Rear(f);

        p=new NODE<Create_PDR>;
        p->info.FAR_ID=f->info.FAR_ID;//关联FAR
        p->info.PDR_ID=10+i;
        p->info.pdi.UE_IP_Address=ue_ip;
        p->info.pdi.Network_Instance=nw_ins;//VN组名
        p->info.rep_detc_info.DCARONI=false;
        create_pdr_all.Insert_Rear(p);
    }
}
