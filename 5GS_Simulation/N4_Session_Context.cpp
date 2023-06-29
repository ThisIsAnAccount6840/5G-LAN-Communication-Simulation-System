#include "N4_Session_Context.h"

N4_SESSION_CONTEXT::N4_SESSION_CONTEXT(){
    UE_IP="NULL";//组级N4会话的UE_IP设置为NULL
}

NODE<Create_FAR> *N4_SESSION_CONTEXT::find_FAR_with_ID(int far_id){
    NODE<Create_FAR> *f;
    for(f=FAR_LIST.head->link; f!=nullptr; f=f->link){
        if(f->info.FAR_ID==far_id) return f;
    }
    return nullptr;
}

Create_PDR* N4_SESSION_CONTEXT::find_matching_pdr_in_N4SessionContext(PDI &pdi){
    NODE<Create_PDR> *temp;
    for(temp=PDR_LIST.head->link; temp!=nullptr; temp=temp->link){
        if(temp->info.pdi == pdi) return &(temp->info);
    }
    return nullptr;
}

void N4_SESSION_CONTEXT::print_N4SessionContext(){
    qDebug()<<"*****Print N4 Session Context*****"<<endl;
    NODE<Create_FAR> *f;
    NODE<Create_PDR> *p;
    for(p=PDR_LIST.head->link;p!=nullptr;p=p->link){
        qDebug()<<"***PDR*** "<<endl;
        qDebug()<<"PDI: "<<endl;
        p->info.pdi.print_pdi();
        qDebug()<<"***FAR***"<<endl;
        qDebug()<<"FAR ID in PDR: "<<p->info.FAR_ID<<endl;
        f=find_FAR_with_ID(p->info.FAR_ID);
        f->info.print_far();
    }
}
