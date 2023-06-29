#include "ue.h"

UE::UE(QObject *parent, QString name, QString s,QString ip, QString mac,bool LAN_C, bool multi, Ui::MainWindow *ui_p) : QObject(parent),ue_w(nullptr,name)
{
    mm_state=MM_DEREGISTERED;
    supi=s;
    IP_addr=ip;
    MAC_addr=mac;
    MM_State=MM_STATE::MM_DEREGISTERED;
    LAN_Capa=LAN_C;
    IS_in_Multicast_Group=multi;
    reg_req=nullptr;

    DNN_input="NULL";
    SST_input=-1;
    SD_input=-1;

    req_t=Existing_PDU_Session;

    ui=ui_p;
    ue_w.send_packet_window.other_ue_info_list=&other_member_info_list;
    mode=0;
    ue_w.PDU_win.setWindowTitle(name+" PDU Session Establishment");
    ue_w.send_packet_window.setWindowTitle(name+" Send Packet Window");
}

UE::~UE(){
    delete reg_req;
    delete est_req;
}

void UE::setSUPI(QString &s){
    supi=s;
}

void UE::setIP(QString &ip){
    IP_addr=ip;
}

void UE::setMAC(QString &mac){
    MAC_addr=mac;
}

void UE::setState(MM_STATE &s){
    MM_State=s;
}

void UE::setLANGroupName(QString & name){
    LAN_Group_Name=name;
}

void UE::setGroupMemberID(QString & id){
    Group_Member_ID=id;
}

void UE::setLANCap(bool c){
    LAN_Capa=c;
}

QString UE::getSUPI(){
    return supi;
}

QString UE::getIP(){
    return IP_addr;
}

QString UE::getMAC(){
    return MAC_addr;
}

MM_STATE UE::getState(){
    return MM_State;
}

void UE::getLANGroupName(){
    LAN_Group_Name.print();
}

void UE::getGroupMemberID(){
    Group_Member_ID.print();
}

bool UE::getLANCapa(){
    return LAN_Capa;
}

void UE::sendRegisReq()
{
    mm_state=MM_REGISTER_INITIATED;
    reg_req=new REGISTRATION_REQUEST(supi, LAN_Capa, Group_Member_ID, IP_addr);
    qDebug()<<"["<<supi<<"]"<<"send Registration_Request..."<<endl;
    ui->Output_Window->append("["+supi+"] send Registration_Request..."+'\n');
    qDebug()<<"["<<supi<<"]"<<"switches to [MM_REGISTER_INITIATED] State"<<endl;
    ui->Output_Window->append("["+supi+"] switches to [MM_REGISTER_INITIATED] State"+'\n');
    delay();
    emit Registration_Request_Msg(reg_req);
}

void UE::recv_Regis_Accept(QString s, LAN_INFO* *lan_config, LINK_LIST<QString>* member_ids){
    if(s==supi){
        mm_state=MM_REGISTERED;
        Group_Member_ID=*member_ids;
        qDebug()<<"["<<supi<<"]"<<" UE Registration Successful"<<endl;
        ui->Output_Window->append("["+supi+"] UE Registration Successful!"+"\n");
        qDebug()<<"["<<supi<<"]"<<"switches to [MM_REGISTERED] State"<<endl;
        ui->Output_Window->append("["+supi+"] switches to [MM_REGISTERED] State"+"\n");
        qDebug()<<"5GLAN Groups that this UE can access:"<<endl;
        ui->Output_Window->append("5GLAN Groups that this UE can access:\n");
        LAN_INFO *grp1, *grp2;
        grp1=lan_config[0];
        grp2=lan_config[1];
        if(grp1!=nullptr) {
            qDebug()<<"Group1: "<<grp1->LAN_Group_Name<<endl;
            ui->Output_Window->append("Group1: "+grp1->LAN_Group_Name+"\n");
            qDebug()<<"DNN: "<<grp1->DNN<<endl;
            ui->Output_Window->append("DNN: "+grp1->DNN+"\n");
            qDebug()<<"S-NSSAI: "<<grp1->SST<<","<<grp1->SD<<endl;
            ui->Output_Window->append("S-NSSAI: "+QString::number(grp1->SST)+","+QString::number(grp1->SD)+"\n");
            qDebug()<<"This UE's Member ID in this group: "<<Group_Member_ID.head->link->info<<endl;
            ui->Output_Window->append("This UE's Member ID in this group: "+Group_Member_ID.head->link->info+"\n");
            qDebug()<<"Members in this group (Member ID): "<<endl;
            ui->Output_Window->append("Members in this group(Member ID):\n");
            grp1->LAN_Member_ID.print();
        }
        if(grp2!=nullptr) {
            qDebug()<<"Group2: "<<grp2->LAN_Group_Name<<endl;
            ui->Output_Window->append("Group2: "+grp2->LAN_Group_Name+"\n");
            qDebug()<<"DNN: "<<grp2->DNN<<endl;
            ui->Output_Window->append("DNN: "+grp2->DNN+"\n");
            qDebug()<<"S-NSSAI: "<<grp2->SST<<","<<grp2->SD<<endl;
            ui->Output_Window->append("S-NSSAI: "+QString::number(grp2->SST)+","+QString::number(grp2->SD)+"\n");
            qDebug()<<"This UE's Member ID in this group: "<<Group_Member_ID.head->link->link->info<<endl;
            ui->Output_Window->append("This UE's Member ID in this group: "+Group_Member_ID.head->link->link->info+"\n");
            qDebug()<<"Members in this group (Member ID): "<<endl;
            ui->Output_Window->append("Members in this group(Member ID):\n");
            grp2->LAN_Member_ID.print();
        }
        if(s=="208950000000031") ui->UE0_Label->show();
        if(s=="208950000000032") ui->UE1_Label->show();
        if(s=="208950000000033") ui->UE2_Label->show();
        if(s=="208950000000034") ui->UE3_Label->show();
        if(s=="208950000000035") ui->UE4_Label->show();
        if(s=="208950000000036") ui->UE5_Label->show();
    }
}

void UE::recv_pdu_est_info(){
    if(DNN_input!=ue_w.PDU_win.DNN_Input.text()){
        DNN_input=ue_w.PDU_win.DNN_Input.text();
        req_t=Initial_Request;
    }
    if(SST_input!=(ue_w.PDU_win.SST_Input.text()).toInt()){
        SST_input=(ue_w.PDU_win.SST_Input.text()).toInt();
        req_t=Initial_Request;
    }
    if(SD_input!=(ue_w.PDU_win.SD_Input.text()).toInt()){
        SD_input=(ue_w.PDU_win.SD_Input.text()).toInt();
        req_t=Initial_Request;
    }

    QString member_ID_Input=ue_w.PDU_win.member_id.toPlainText();
    //qDebug()<<"Member ID Input: "<<member_ID_Input<<endl;
    int first=member_ID_Input.indexOf(',');
    NODE<QString> *member_id;
    Member_ID_Input_List.Make_Empty();//清空上一次输入
    while(first!=-1){
        member_id=new NODE<QString>(member_ID_Input.left(first));
        Member_ID_Input_List.Insert_Rear(member_id);
        member_ID_Input.remove(0,first+1);//把‘,’也删了
        //默认输入是符合id name1,id name2,...的格式
        first=member_ID_Input.indexOf(',');
    }
    member_id=new NODE<QString>(member_ID_Input);
    Member_ID_Input_List.Insert_Rear(member_id);
    //qDebug()<<"Link List:"<<endl;
    //Member_ID_Input_List.print();

    ue_w.PDU_win.hide();
    ue_w.Start.show();
    //在此函数中发出PDU会话建立请求
    send_PDU_Session_Est_Req();

    //qDebug()<<DNN_input<<" "<<SST_input<<" "<<SD_input<<" "<<endl;
}

void UE::PDU_Window_Show(){
    if(mm_state == MM_REGISTERED){
        ue_w.PDU_win.show();
    }
    else{
        ue_w.error_win.show();
    }
}

void UE::send_PDU_Session_Est_Req(){
    qDebug()<<"["<<supi<<"]"<<"send PDU Session Establishment Request..."<<endl;
    ui->Output_Window->append("["+supi+"] send PDU Session Establishment Request..."+"\n");
    delay();
    NODE<QString> *id=new NODE<QString>(supi+DNN_input);
    PDU_Session_ID.Insert_Rear(id);
    est_req=new PDU_SESSION_EST_REQ(supi+DNN_input, DNN_input, SST_input, SD_input, Member_ID_Input_List);
    est_req->UE_IP_Addre=IP_addr;
    est_req->in_Multicast_Group=IS_in_Multicast_Group;
    est_req->req_type=req_t;
    emit PDU_Session_Esta_Req(est_req);
}

void UE::handle_AN_Specific_Resoure_Setup(PDU_SESSION_ESTABLISHMENT_ACCEPT * acpt, QString s){
    if(s!=supi) return;
    NODE<other_ue_info> *other=new NODE<other_ue_info>;
    other->info.PDUSession_ID=PDU_Session_ID.tail->info;
    NODE<MAC_Info> *new_mac_info, *temp;
    for(temp=acpt->member_mac_addre->head->link; temp!=nullptr; temp=temp->link){
        //qDebug()<<"****"<<temp->info.Member_ID<<endl;
        new_mac_info=new NODE<MAC_Info>(*temp);
        other->info.other_member_mac_info_list.Insert_Rear(new_mac_info);
    }
    //qDebug()<<"***new other ue info"<<endl;
    //other->info.print_other_ue_info();
    other_member_info_list.Insert_Rear(other);
    acpt->member_mac_addre->Make_Empty();//清空GMF中的MAC_Info_to_UE参数，以便后面的UE使用
    NODE<other_ue_info> *tp;
    for(tp=other_member_info_list.head->link; tp!=nullptr; tp=tp->link){
        qDebug()<<"=====PRINT OTHER UE INFO====="<<endl;
        tp->info.print_other_ue_info();
    }
    if(s=="208950000000036"){ //N6连接的UE不需要后续的步骤，直接获得其他UE info后结束PDU会话建立
        qDebug()<<"=====PDU Session Establishment Complete!====="<<endl;
        ui->Output_Window->append("=====PDU Session Establishment Complete!=====\n");
        ui->UE5_Label_2->show();
        return;
    }
    else emit AN_Specific_Resource_Setup_UE();
}

void UE::UE_send_packet(QString sid){
    ue_data_packet=new Data_Packet;
    ue_data_packet->src_ip=IP_addr;
    ue_data_packet->dst_ip=dst_ip_input;
    ue_data_packet->TEID=-1;//N6
    ue_data_packet->src_interface=-1;
    ue_data_packet->T_PDU=content_input;
    if(sid!="null"){//广播/组播
        if(sid.right(4)=="VN01") ue_data_packet->network_instance="VN_Group1";
        if(sid.right(4)=="VN02") ue_data_packet->network_instance="VN_Group2";
        delay();
        emit Packet_sent_by_UE(*ue_data_packet, sid);
    }
    else{//单播.寻找该使用的PDU Session ID从而得知在哪个组内通信
        NODE<other_ue_info> *temp;
        NODE<MAC_Info> *mac_node;
        for(temp=other_member_info_list.head->link; temp!=nullptr; temp=temp->link){
            for(mac_node=temp->info.other_member_mac_info_list.head->link; mac_node!=nullptr; mac_node=mac_node->link){
                if(ue_data_packet->dst_ip==mac_node->info.MAC_Addr){
                    if(temp->info.PDUSession_ID.right(4)=="VN01"){
                        ue_data_packet->network_instance="VN_Group1";
                    }
                    if(temp->info.PDUSession_ID.right(4)=="VN02"){
                        ue_data_packet->network_instance="VN_Group2";
                    }
                    qDebug()<<"["<<supi<<"]"<<"send a packet..."<<endl;
                    ui->Output_Window->append("["+supi+"] send a packet..."+"\n");
                    delay();
                    emit Packet_sent_by_UE(*ue_data_packet, temp->info.PDUSession_ID);
                    return;
                }
            }
        }
    }
}

void UE::UE_recv_packet(Data_Packet &data){
    if(data.dst_ip!=IP_addr) return;
    else{
        qDebug()<<"["<<supi<<"]"<<"received a packet: "<<endl;
        ui->Output_Window->append("["+supi+"] received a packet:\n");
        qDebug()<<data.T_PDU<<endl;
        ui->Output_Window->append(data.T_PDU+"\n");
        delete &data;
    }

}

void UE::sendPacketWindow_show(){
    /*
    NODE<other_ue_info> *temp;
    NODE<MAC_Info> *mac_node;
    int i;
    for(i=0, temp=other_member_info_list.head->link; temp!=nullptr; temp=temp->link){
        for(mac_node=temp->info.other_member_mac_info_list.head->link; mac_node!=nullptr; mac_node=mac_node->link){
            if(ue_w.send_packet_window.unicast_buttons[i]->text()=="null") {
                ue_w.send_packet_window.unicast_buttons[i]->setText(mac_node->info.Member_ID);
                ue_w.send_packet_window.member_ips[i]=mac_node->info.MAC_Addr;
            }
            ++i;
        }
    }
    */
    ue_w.send_packet_window.show();
}

void UE::recv_packet_info(){
    if(mode==2) {//广播
        dst_ip_input="255.255.255.255";
        qDebug()<<"["<<supi<<"]"<<"send a broadcast packet..."<<endl;
        ui->Output_Window->append("["+supi+"] send a broadcast packet...\n");
        content_input=ue_w.send_packet_window.content.toPlainText();
        qDebug()<<"The data to be transmitted: "<<content_input<<endl;
        ui->Output_Window->append("The data to be transmitted: "+content_input+"\n");

        NODE<QString> *pduID=PDU_Session_ID.head->link;
        if(ue_w.send_packet_window.group1_select.isChecked()==true) {
            if(pduID->info.right(4)!="VN01") pduID=pduID->link;
        }
        if(ue_w.send_packet_window.group2_select.isChecked()==true) {
            if(pduID->info.right(4)!="VN02") pduID=pduID->link;
        }
        UE_send_packet(pduID->info);
        return;
    }
    if(mode==1){//组播
        if(IS_in_Multicast_Group==false) return;
        dst_ip_input="244.1.1.1";
        NODE<QString> *pduID=PDU_Session_ID.head->link;
        if(pduID->info.right(4)!="VN01") pduID=pduID->link;//针对UE2
        qDebug()<<"["<<supi<<"]"<<"send a multicast packet..."<<endl;
        ui->Output_Window->append("["+supi+"] send a multicast packet...\n");
        content_input=ue_w.send_packet_window.content.toPlainText();
        qDebug()<<"The data to be transmitted: "<<content_input<<endl;
        ui->Output_Window->append("The data to be transmitted: "+content_input+"\n");
        UE_send_packet(pduID->info);
        return;
    }
    //单播
    int i;
    QString member_ID="null";
    for(i=0;i<5;i++){
        if(ue_w.send_packet_window.unicast_buttons[i]->isChecked()==true){
            member_ID=ue_w.send_packet_window.unicast_buttons[i]->text();
            break;
        }
    }
    if(member_ID=="null") qDebug()<<"ERROR: NO MEMBER ID SELECTED"<<endl;
    else{
        dst_ip_input=ue_w.send_packet_window.member_ips[i];
        qDebug()<<supi<<":"<<IP_addr<<"->"<<ue_w.send_packet_window.unicast_buttons[i]->text()<<":"<<dst_ip_input<<endl;
        ui->Output_Window->append("["+supi+"] send a unicast packet:\n");
    }
    content_input=ue_w.send_packet_window.content.toPlainText();
    qDebug()<<"The data to be transmitted: "<<content_input<<endl;
    ui->Output_Window->append("The data to be transmitted: "+content_input+"\n");
    UE_send_packet();
}

void UE::unicast_member_show(){
    //组播和广播相关信息隐藏
    ue_w.send_packet_window.prompt.hide();
    ue_w.send_packet_window.member1.hide();
    ue_w.send_packet_window.member2.hide();
    ue_w.send_packet_window.member3.hide();
    ue_w.send_packet_window.group1_select.hide();
    ue_w.send_packet_window.group2_select.hide();

    ue_w.send_packet_window.content_label.move(20,200);
    ue_w.send_packet_window.content.move(20, 225);
    ue_w.send_packet_window.start.move(30,500);
    //set button text
    NODE<other_ue_info> *group;
    NODE<MAC_Info> *mac_node;
    int i=0;
    for(group=other_member_info_list.head->link; group!=nullptr; group=group->link){
        for(mac_node=group->info.other_member_mac_info_list.head->link; mac_node!=nullptr; mac_node=mac_node->link){
            if(ue_w.send_packet_window.unicast_buttons[i]->text()=="null"){
                ue_w.send_packet_window.unicast_buttons[i]->setText(mac_node->info.Member_ID);
                ue_w.send_packet_window.member_ips[i]=mac_node->info.MAC_Addr;
                ue_w.send_packet_window.unicast_buttons[i]->show();
                switch (i)
                {
                case 0:
                    ue_w.send_packet_window.unicast_buttons[i]->move(20,50);
                    break;
                case 1:
                    ue_w.send_packet_window.unicast_buttons[i]->move(20,100);
                    break;
                case 2:
                    ue_w.send_packet_window.unicast_buttons[i]->move(20,150);
                    break;
                case 3:
                    ue_w.send_packet_window.unicast_buttons[i]->move(220,50);
                    break;
                case 4:
                    ue_w.send_packet_window.unicast_buttons[i]->move(220,100);
                    break;
                }
            }
            else ue_w.send_packet_window.unicast_buttons[i]->show();
            ++i;
            if(i>4) break;
        }
    }
    /*
    //显示上述被设置过的按钮
    for(i=0;i<5;i++){
        if(ue_w.send_packet_window.unicast_buttons[i]->text()!="null"){
            ue_w.send_packet_window.unicast_buttons[i]->show();
            switch (i)
            {
            case 0:
                ue_w.send_packet_window.unicast_buttons[i]->move(20,50);
                break;
            case 1:
                ue_w.send_packet_window.unicast_buttons[i]->move(20,100);
                break;
            case 2:
                ue_w.send_packet_window.unicast_buttons[i]->move(20,150);
                break;
            case 3:
                ue_w.send_packet_window.unicast_buttons[i]->move(220,50);
                break;
            case 4:
                ue_w.send_packet_window.unicast_buttons[i]->move(220,100);
                break;
            }

        }
    }*/
    ue_w.send_packet_window.content.show();
    ue_w.send_packet_window.content_label.show();
}

void UE::broadcast_group_show(){
    mode=2;
    //单播，组播信息隐藏
    int i;
    for(i=0;i<5;i++) ue_w.send_packet_window.unicast_buttons[i]->hide();
    ue_w.send_packet_window.prompt.hide();
    ue_w.send_packet_window.member1.hide();
    ue_w.send_packet_window.member2.hide();
    ue_w.send_packet_window.member3.hide();

    ue_w.send_packet_window.content.move(180,225);
    ue_w.send_packet_window.content.show();
    ue_w.send_packet_window.content_label.move(180,200);
    ue_w.send_packet_window.content_label.show();
    ue_w.send_packet_window.start.move(200,500);
    NODE<QString> *pduID=PDU_Session_ID.head->link;
    if(pduID->link!=nullptr){//UE2建立了两个PDU会话
        ue_w.send_packet_window.group1_select.show();
        ue_w.send_packet_window.group2_select.show();
    }
}

void UE::multicast_member_show(){
    //单播信息隐藏
    int i;
    for(i=0;i<5;i++) ue_w.send_packet_window.unicast_buttons[i]->hide();
     mode=1;
     if(IS_in_Multicast_Group==false){
        ue_w.multi_error_win.show();
        return;
    }
    ue_w.send_packet_window.prompt.show();
    ue_w.send_packet_window.member1.show();
    ue_w.send_packet_window.member2.show();
    ue_w.send_packet_window.member3.show();
    ue_w.send_packet_window.content.move(180,225);
    ue_w.send_packet_window.content_label.move(180,200);
    ue_w.send_packet_window.content.show();
    ue_w.send_packet_window.content_label.show();
    ue_w.send_packet_window.start.move(200,500);
}

void UE::send_broadcast_packet(){
    dst_ip_input="255.255.255.255";
    qDebug()<<"["<<supi<<"]"<<"send a broadcast packet..."<<endl;
    content_input=ue_w.send_packet_window.content.toPlainText();
    qDebug()<<"The data to be transmitted: "<<content_input<<endl;

    NODE<QString> *pduID=PDU_Session_ID.head->link;
    if(ue_w.send_packet_window.group1_select.isChecked()==true) {
        if(pduID->info.right(4)!="VN01") pduID=pduID->link;
    }
    if(ue_w.send_packet_window.group2_select.isChecked()==true) {
        if(pduID->info.right(4)!="VN02") pduID=pduID->link;
    }
    UE_send_packet(pduID->info);
}

void UE::PDU_Success_Label_Show(QString s){
    if(s=="208950000000031") ui->UE0_Label_2->show();
    if(s=="208950000000032") ui->UE1_Label_2->show();
    if(s=="208950000000033") ui->UE2_Label_2->show();
    if(s=="208950000000034") ui->UE3_Label_2->show();
    if(s=="208950000000035") ui->UE4_Label_2->show();
    if(s=="208950000000036") ui->UE5_Label_2->show();
}
