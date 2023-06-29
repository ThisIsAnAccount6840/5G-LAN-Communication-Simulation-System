#include "mac_info.h"


MAC_Info::MAC_Info()
{

}

void MAC_Info::print_mac_info(){
    qDebug()<<"SUPI: "<<SUPI<<endl;
    qDebug()<<"Member ID: "<<Member_ID<<endl;
    qDebug()<<"Address :"<<MAC_Addr<<endl;
    qDebug()<<"========================"<<endl;
}

void other_ue_info::print_other_ue_info(){
    qDebug()<<"PDU Session ID: "<<PDUSession_ID<<endl;
    NODE<MAC_Info> *temp;
    for(temp=other_member_mac_info_list.head->link; temp!=nullptr; temp=temp->link){
        temp->info.print_mac_info();
    }
}
