#include "gmf.h"

GMF::GMF(QObject *parent, int n, QString ue_supi_list[], LAN_INFO *all_lan_info, Ui::MainWindow* ui_p) : QObject(parent)
{
    ui=ui_p;
    int i;
    for(i=0;i<n;i++) add_supi_to_lan_list(ue_supi_list[i]);
    group1_config=all_lan_info[0];
    //qDebug()<<"GMF构造函数"<<endl;
    //group1_config.LAN_Member_ID.print();
    group2_config=all_lan_info[1];
    LAN_Group_Config[0]=nullptr;
    LAN_Group_Config[1]=nullptr;
    group1_memberID_list=all_lan_info[0].LAN_Member_ID;
    group2_memberID_list=all_lan_info[1].LAN_Member_ID;
}

GMF::~GMF(){

}

void GMF::add_supi_to_lan_list(QString s){
    NODE<QString> *node=new NODE<QString>(s);
    ue_with_lan_cap_list.Insert_Rear(node);
}

void GMF::add_memberID_to_group1(QString id){
    NODE<QString> *node=new NODE<QString>(id);
    group1_memberID_list.Insert_Rear(node);
}

void GMF::add_memberID_to_group2(QString id){
    NODE<QString> *node=new NODE<QString>(id);
    group2_memberID_list.Insert_Rear(node);
}

bool GMF::find_supi_in_lan_list(QString s){
    if(ue_with_lan_cap_list.find_node(s)!=nullptr) return true;
    else return false;
}

bool GMF::find_member_id(int num, LINK_LIST<QString> &ids, LINK_LIST<QString> &group_names){
    int i;
    NODE<QString> *node, *temp_id, *temp_group;
    bool result=true;
    for(i=0,temp_id=ids.head->link, temp_group=group_names.head->link;i<num;i++){
        if(temp_group->info==group1_memberID_list.Name){
            node=group1_memberID_list.find_node(temp_id->info);
            //qDebug()<<"find node已执行"<<endl;
            if(node==nullptr){
                result=false;
                qDebug()<<"ERROR: Member ID: "<<temp_id->info<<" Not Found in Group "<<group1_memberID_list.Name<<endl;
                ui->Output_Window->append("ERROR: Member ID:"+temp_id->info+" Not Found in Group "+group1_memberID_list.Name+"\n");
            }
        }
        else if(temp_group->info==group2_memberID_list.Name){
            node=group2_memberID_list.find_node(temp_id->info);
            if(node==nullptr){
                result=false;
                qDebug()<<"ERROR: Member ID: "<<temp_id->info<<" Not Found in Group "<<group2_memberID_list.Name<<endl;
                ui->Output_Window->append("ERROR: Member ID:"+temp_id->info+" Not Found in Group "+group2_memberID_list.Name+"\n");
            }
        }
        else{
            qDebug()<<"ERROR: Group Name Not Found!"<<endl;
            ui->Output_Window->append("ERROR: Group Name Not Found!\n");
            return false;
        }
        temp_group=temp_group->link;
        temp_id=temp_id->link;
    }
    return result;
}

void GMF::handle_group_regis_msg(int num, QString s, LINK_LIST<QString> id, LINK_LIST<QString> group, QString mac){
    qDebug()<<"[GMF] Group Authentication Request Received"<<endl;
    ui->Output_Window->append("[GMF] Group Authentication Request Received\n");
    qDebug()<<"[GMF] Group Authentication Procedure Starts..."<<endl;
    ui->Output_Window->append("[GMF] Group Authentication Procedure Starts...\n");
    bool result;
    if(find_supi_in_lan_list(s)){
        delay();
        qDebug()<<"     LAN Capability Authenticated..."<<endl;
        ui->Output_Window->append("     LAN Capability Authenticated...\n");
        if(find_member_id(num, id, group)){
            delay();
            qDebug()<<"     Member ID Authenticated..."<<endl;
            ui->Output_Window->append("     Member ID Authenticated...\n");
            qDebug()<<"     Group Authentication Succeeded"<<endl;
            ui->Output_Window->append("     Group Authentication Succeeded\n");
            result=true;

            NODE<QString> *temp_g, *temp_i;
            int i;
            LAN_Group_Config[0]=nullptr;
            LAN_Group_Config[1]=nullptr;
            for(i=0,temp_g=group.head->link, temp_i=id.head->link;i<num;i++){
                if(temp_g->info==group1_memberID_list.Name) {
                    LAN_Group_Config[i]=&group1_config;
                    NODE<MAC_Info> *mac_node=new NODE<MAC_Info>;
                    mac_node->info.MAC_Addr=mac;
                    mac_node->info.Member_ID=temp_i->info;
                    mac_node->info.SUPI=s;
                    group1_member_mac_info.Insert_Rear(mac_node);//把已注册了的UE的相关信息存在GMF中
                }
                else {
                    LAN_Group_Config[i]=&group2_config;
                    NODE<MAC_Info> *mac_node=new NODE<MAC_Info>;
                    mac_node->info.MAC_Addr=mac;
                    mac_node->info.Member_ID=temp_i->info;
                    mac_node->info.SUPI=s;
                    group2_member_mac_info.Insert_Rear(mac_node);
                }
                temp_g=temp_g->link;
                temp_i=temp_i->link;
            }
            qDebug()<<"[GMF] send Group_Authentication_Response..."<<endl;
            ui->Output_Window->append("[GMF] send Group_Authentication_Response...\n");
            delay();
            emit Group_Registration_Response(result, LAN_Group_Config, &id);
        }
        else{
            qDebug()<<"ERROR: Member ID Not Found"<<endl;
            ui->Output_Window->append("ERROR: Member ID Not Found\n");
            result=false;
            LINK_LIST<QString> null;
            emit Group_Registration_Response(result, nullptr, &null);
        }
    }
    else{
        qDebug()<<"ERROR: LAN Not Supported for This UE"<<endl;
        ui->Output_Window->append("ERROR: LAN Not Supported for This UE\n");
        result=false;
        LINK_LIST<QString> null;
        emit Group_Registration_Response(result, nullptr, &null);
    }

}

void GMF::handle_PDU_Session_Regis_Req(QString sid, LINK_LIST<QString> & m_ids){
    qDebug()<<"[GMF] PDU Session Registration Request Received"<<endl;
    ui->Output_Window->append("[GMF] PDU Session Registration Request Received\n");
    NODE<QString> *node=new NODE<QString>(sid);
    NODE<MAC_Info> *mac_node, *new_mac_node;
    QString Group_Name;
    GMF_PDU_Session_ID.Insert_Rear(node);
    for(node=m_ids.head->link; node!=nullptr; node=node->link){
        if(group1_memberID_list.find_node(node->info)){
            if(sid.right(4)!="VN01") {//验证ID是否在当前PDU会话对应的组内
                qDebug()<<"ERROR: Wrong Member ID Input!"<<endl;
                ui->Output_Window->append("ERROR: Wrong Member ID Input!\n");
                return;
            }
            Group_Name=group1_memberID_list.Name;//VN组名
            for(mac_node=group1_member_mac_info.head->link; mac_node!=nullptr; mac_node=mac_node->link){
                if(mac_node->info.Member_ID==node->info){
                    new_mac_node=new NODE<MAC_Info>(*mac_node);
                    MAC_Info_to_UE.Insert_Rear(new_mac_node);
                    break;
                }
            }
        }
        else if(group2_memberID_list.find_node(node->info)){
            if(sid.right(4)!="VN02") {//验证ID是否在当前PDU会话对应的组内
                qDebug()<<"ERROR: Wrong Member ID Input!"<<endl;
                ui->Output_Window->append("ERROR: Wrong Member ID Input!\n");
                return;
            }
            Group_Name=group2_memberID_list.Name;
            for(mac_node=group2_member_mac_info.head->link; mac_node!=nullptr; mac_node=mac_node->link){
                if(mac_node->info.Member_ID==node->info){
                    new_mac_node=new NODE<MAC_Info>(*mac_node);
                    MAC_Info_to_UE.Insert_Rear(new_mac_node);
                    break;
                }
            }
        }
        else {
            qDebug()<<"ERROR: Wrong Member ID Input!"<<endl;
            ui->Output_Window->append("ERROR: Wrong Member ID Input!\n");
            return;
        }
    }
    qDebug()<<"[GMF] send PDU Session Registration Response..."<<endl;
    ui->Output_Window->append("[GMF] send PDU Session Registration Response...\n");
    delay();
    emit PDU_Session_Registration_Response(&MAC_Info_to_UE, Group_Name);
}
