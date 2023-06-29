#include "udm.h"

UDM::UDM(QObject *parent) : QObject(parent)
{
    //ue_sub_data_base_head=new LINK_LIST<UE_Sub_Data>;
}

UDM::~UDM(){
    //delete ue_sub_data_base_head;
}

void UDM::handle_Get_UE_Sub_Data(QString s){
    UE_Sub_Data *u;
    u=find_ue_sub_data(s);
    emit UE_Sub_Data_send(u);
}

void UDM::add_ue_record(UE &ue, int UE_LAN_n, LINK_LIST<QString> &UE_LAN_Group_Name, LINK_LIST<QString> &UE_Member_ID, LINK_LIST<QString> &UE_DNN, LINK_LIST<int> &UE_SST, LINK_LIST<int> &UE_SD){
    NODE<UE_Sub_Data>* new_ue_info=new NODE<UE_Sub_Data>;
    new_ue_info->info.SUPI=ue.getSUPI();
    new_ue_info->info.LAN_CAP=ue.getLANCapa();
    new_ue_info->info.LAN_num=UE_LAN_n;
    new_ue_info->info.LAN_Group_Name=UE_LAN_Group_Name;
    new_ue_info->info.LAN_Member_ID=UE_Member_ID;
    new_ue_info->info.DNN_list=UE_DNN;
    new_ue_info->info.SST=UE_SST;
    new_ue_info->info.SD=UE_SD;
    ue_sub_data_base.Insert_Rear(new_ue_info);
}

UE_Sub_Data* UDM::find_ue_sub_data(QString s){
    NODE<UE_Sub_Data> *temp=ue_sub_data_base.head->link;
    for(;temp!=nullptr;temp=temp->link){
        if(temp->info.SUPI==s){
            return &(temp->info);
        }
    }
    return nullptr;
}
