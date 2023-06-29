#include "smf.h"

SMF::SMF(QObject *parent, LINK_LIST<QString> &group1_supi_list, LINK_LIST<QString> &group2_supi_list, Ui::MainWindow *ui_p) : QObject(parent)
{
    group1_member_supi_list=group1_supi_list;
    group1_member_num=group1_member_supi_list.node_num();
    group2_member_supi_list=group2_supi_list;
    group2_member_num=group2_member_supi_list.node_num();
    ui=ui_p;
}

SMF::~SMF(){
    //delete n4_session_est_req;
}

void SMF::handle_CreateSMContext_Req(PDU_SESSION_EST_REQ * est){
    qDebug()<<"[SMF] PDUSession_CreateSMContext_Request Received"<<endl;
    ui->Output_Window->append("[SMF] PDUSession_CreateSMContext_Request Received\n");
    NODE<QString> *id=new NODE<QString>(est->PDU_Session_ID);
    SMF_PDU_Session_ID.Insert_Rear(id);
    UE_IP_Add=est->UE_IP_Addre;
    is_in_multicast_group=est->in_Multicast_Group;
    if(UE_IP_Add=="12.1.1.6") is_ue5_in_multicast_group=est->in_Multicast_Group;//UE5发来的请求
    qDebug()<<"[SMF] send PDU Session Registration Request..."<<endl;
    ui->Output_Window->append("[SMF] send PDU Session Registration Request...\n");
    delay();
    emit PDU_Session_Registration_Request(est->PDU_Session_ID, est->Member_IDs);
}

void SMF::PDU_Session_Regis_Response_Recv(LINK_LIST<MAC_Info> *member_mac, QString group_name){
    qDebug()<<"[SMF] PDU Session Registration Response Received"<<endl;
    ui->Output_Window->append("[SMF] PDU Session Registration Response Received\n");
    Member_MAC_Address_to_UE=member_mac;
    est_req=new N4_SESSION_ESTABLISHMENT_REQUEST(UE_IP_Add, group_name, is_in_multicast_group);
    qDebug()<<"[SMF] send N4 Session Establishment Request..."<<endl;
    ui->Output_Window->append("[SMF] send N4 Session Establishment Request...\n");
    delay();
    emit N4_Session_Est_Req(est_req);
}

void SMF::handle_N4_Session_Est_Response(int teid){
    qDebug()<<"[SMF] N4 Session Establishment Response Received"<<endl;
    ui->Output_Window->append("[SMF] N4 Session Establishment Response Received\n");
    pdu_session_est_accept.TEID=teid;
    pdu_session_est_accept.member_mac_addre=Member_MAC_Address_to_UE;
    QString supi=SMF_PDU_Session_ID.tail->info.left(15);
    if(supi=="208950000000036"){//只要UE5进行PDU会话的建立就触发组级N4会话的配置
        //只进行N6相关的转发规则的配置
        group1_member_num=-1;
        N4_SESSION_ESTABLISHMENT_REQUEST *group_n4_est_req=new N4_SESSION_ESTABLISHMENT_REQUEST;
        group_n4_est_req->node_num=2;//N6(2)
        int PDR_ID=100;
        int FAR_ID=200;
        QString ue5_ip="12.1.1.6";
        NODE<Create_FAR> *f=group_n4_est_req->create_far_all.head->link;
        NODE<Create_PDR> *p=group_n4_est_req->create_pdr_all.head->link;
        //N6上行PDI&FAR的配置：
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=1;//core side
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue5_ip;
        p->info.pdi.Network_Instance="VN_Group1";
        //FAR
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal
        //N6单播下行PDI&FAR的配置：
        p=p->link;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue5_ip;
        p->info.pdi.Network_Instance="VN_Group1";
        //FAR
        f=f->link;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=1;//core side
        //N6广播下行PDR&FAR的配置
        p=new NODE<Create_PDR>;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.FAR_ID=FAR_ID;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address="255.255.255.255";
        p->info.pdi.Network_Instance="VN_Group1";
        //FAR
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="11";
        f->info.duplicating_parameters.Destination_Interface=1;//core side
        group_n4_est_req->create_pdr_all.Insert_Rear(p);
        group_n4_est_req->create_far_all.Insert_Rear(f);

        if(is_ue5_in_multicast_group==true){//增加N6组播下行PDR
            p=new NODE<Create_PDR>;
            p->info.PDR_ID=PDR_ID;
            PDR_ID++;
            p->info.FAR_ID=FAR_ID-1;//关联到广播下行FAR
            p->info.pdi.Source_Interface=4;//5G VN Internal
            p->info.pdi.Local_FTEID.IP_Address="null";
            p->info.pdi.Local_FTEID.TEID=-1;
            p->info.pdi.UE_IP_Address="244.1.1.1";
            p->info.pdi.Network_Instance="VN_Group1";
            group_n4_est_req->create_pdr_all.Insert_Rear(p);
        }

        qDebug()<<"[SMF] send Group-level N4 Session Establishment Request to UPF1..."<<endl;
        ui->Output_Window->append("[SMF] send Group-level N4 Session Establishment Request to UPF1...\n");
        delay();
        emit Group_Level_N4_Session_Est_Request_1(group_n4_est_req);
    }
    qDebug()<<"Communication N1N2 Message Transfer"<<endl;
    ui->Output_Window->append("Communication N1N2 Message Transfer\n");
    delay();
    emit N1N2_Message_Transfer_smf(&pdu_session_est_accept, SMF_PDU_Session_ID.tail->info);//PDU会话建立接受的消息被包含在这一步的消息中发送给AMF
}

void SMF::handle_UpdateSMContext_Req(int teid, QString ip){
    qDebug()<<"[SMF] PDU Session Update SMContext Request Received"<<endl;
    ui->Output_Window->append("[SMF] PDU Session Update SMContext Request Received\n");
    qDebug()<<"[SMF] send N4 Session Modification Request..."<<endl;
    ui->Output_Window->append("[SMF] send N4 Session Modification Request...\n");
    delay();
    QString DNN=SMF_PDU_Session_ID.tail->info.right(4);//当前正在进行PDU会话建立的UE的SUPI
    QString group_name;//当前在哪个组内进行PDU会话建立
    QString supi=SMF_PDU_Session_ID.tail->info.left(15);
    if(DNN=="VN01") group_name="VN_Group1";
    if(DNN=="VN02") group_name="VN_Group2";
    emit N4_Session_Modification_Request(teid, ip, group_name, supi);
}

void SMF::handle_N4_Session_Modi_Response(){
    qDebug()<<"[SMF] N4 Session Modification Response Received"<<endl;
    ui->Output_Window->append("[SMF] N4 Session Modification Response Received\n");
    qDebug()<<"=====DOWNLINK CONFIGURATION COMPLETE====="<<endl;
    ui->Output_Window->append("=====DOWNLINK CONFIGURATION COMPLETE=====\n");
    qDebug()<<"[SMF] send PDUSession UpdateSMContext Response..."<<endl;
    ui->Output_Window->append("[SMF] send PDUSession UpdateSMContext Response...\n");
    delay();
    qDebug()<<"[AMF] PDUSession UpdateSMContext Response Received"<<endl;
    ui->Output_Window->append("[AMF] PDUSession UpdateSMContext Response Received\n");
    qDebug()<<"=====PDU Session Establishment Complete!====="<<endl;
    ui->Output_Window->append("=====PDU Session Establishment Complete!=====\n");

    NODE<QString> *sid=SMF_PDU_Session_ID.tail;//当前正在建立的PDU SESSION ID
    QString supi=sid->info.left(15);
    QString dnn=sid->info.right(4);
    emit PDU_Success_Show(supi);
    if(group1_member_supi_list.find_node(supi)&&dnn=="VN01") group1_member_num--;
    if(group2_member_supi_list.find_node(supi)&&dnn=="VN02") group2_member_num--;

    if(supi=="208950000000033"&&dnn=="VN02"){
        //进行N19相关PDR&FAR的配置
        group2_member_num=-1;
        N4_SESSION_ESTABLISHMENT_REQUEST *group1_n4_est_req=new N4_SESSION_ESTABLISHMENT_REQUEST;
        N4_SESSION_ESTABLISHMENT_REQUEST *group2_n4_est_req=new N4_SESSION_ESTABLISHMENT_REQUEST;
        group1_n4_est_req->node_num=5;//UPF1_N19_Unicast(4)+UPF1_N19_Broadcast发送(1)
        group2_n4_est_req->node_num=4;//UPF2_N19_uni(3)+UPF2_N19_broad发送(1)
        int PDR_ID=100;
        int FAR_ID=200;
        int upf1_ue2_n19teid=21;//UPF1为UE2分配的N19隧道TEID
        int upf2_ue3_n19teid=31,upf2_ue4_n19teid=32;//UPF2为UE和UE4分配的N19隧道TEID
        QString ue2_ip="12.1.1.3", ue3_ip="12.1.1.4", ue4_ip="12.1.1.5";
        QString upf1_n19_ip="d", upf2_n19_ip="e";

        //UPF1组级N4会话的配置
        NODE<Create_FAR> *f=group1_n4_est_req->create_far_all.head->link;
        NODE<Create_PDR> *p=group1_n4_est_req->create_pdr_all.head->link;
        //1.N19接收对应PDI&FAR的配置(src:ue3)：
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=1;//core side
        p->info.pdi.Local_FTEID.IP_Address=upf1_n19_ip;
        p->info.pdi.Local_FTEID.TEID=upf1_ue2_n19teid;
        p->info.pdi.UE_IP_Address=ue3_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal
        //2.N19单播发送对应PDI&FAR的配置(dst:UE3)：
        p=p->link;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue3_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f=f->link;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=1;//core side
        f->info.forwarding_parameters.Outer_Header_Creation.IP_Address=upf2_n19_ip;
        f->info.forwarding_parameters.Outer_Header_Creation.TEID=upf2_ue3_n19teid;

        //3.N19接收对应PDI&FAR的配置(src:ue4)：
        p=new NODE<Create_PDR>;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=1;//core side
        p->info.pdi.Local_FTEID.IP_Address=upf1_n19_ip;
        p->info.pdi.Local_FTEID.TEID=upf1_ue2_n19teid;
        p->info.pdi.UE_IP_Address=ue4_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal
        group1_n4_est_req->create_pdr_all.Insert_Rear(p);
        group1_n4_est_req->create_far_all.Insert_Rear(f);
        //4.N19单播发送对应PDI&FAR的配置(dst:UE4)：
        p=new NODE<Create_PDR>;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue4_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=1;//core side
        f->info.forwarding_parameters.Outer_Header_Creation.IP_Address=upf2_n19_ip;
        f->info.forwarding_parameters.Outer_Header_Creation.TEID=upf2_ue4_n19teid;
        group1_n4_est_req->create_pdr_all.Insert_Rear(p);
        group1_n4_est_req->create_far_all.Insert_Rear(f);
        //5.N19广播发送对应PDI&FAR的配置(dst:UE3 and UE4)
        p=new NODE<Create_PDR>;
        p->info.PDR_ID=PDR_ID;
        p->info.FAR_ID=FAR_ID-1;//关联上一个单播发送给UE4的FAR。只需把包发送到对面UPF即可，复制在本地完成
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address="255.255.255.255";
        p->info.pdi.Network_Instance="VN_Group2";
        group1_n4_est_req->create_pdr_all.Insert_Rear(p);

        //UPF2组级N4会话的配置
        f=group2_n4_est_req->create_far_all.head->link;
        p=group2_n4_est_req->create_pdr_all.head->link;
        //1.N19接收对应PDI&FAR的配置(接收后发给ue3)：
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=1;//core side
        p->info.pdi.Local_FTEID.IP_Address=upf2_n19_ip;
        p->info.pdi.Local_FTEID.TEID=upf2_ue3_n19teid;
        p->info.pdi.UE_IP_Address=ue2_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal
        //2.N19单播发送对应PDI&FAR的配置(src:UE3/4,dst:UE2)：
        p=p->link;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue2_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f=f->link;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=1;//core side
        f->info.forwarding_parameters.Outer_Header_Creation.IP_Address=upf1_n19_ip;
        f->info.forwarding_parameters.Outer_Header_Creation.TEID=upf1_ue2_n19teid;
        //3.N19接收对应PDI&FAR的配置(接收后发给ue4)：
        p=new NODE<Create_PDR>;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=1;//core side
        p->info.pdi.Local_FTEID.IP_Address=upf2_n19_ip;
        p->info.pdi.Local_FTEID.TEID=upf2_ue4_n19teid;
        p->info.pdi.UE_IP_Address=ue2_ip;
        p->info.pdi.Network_Instance="VN_Group2";
        //FAR
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal
        group2_n4_est_req->create_pdr_all.Insert_Rear(p);
        group2_n4_est_req->create_far_all.Insert_Rear(f);
        //N19单播发送对应PDI&FAR的配置(src: UE4, dst: UE2)：--该PDR和UE3的重复了所以导致UE4发N19包失败
        /*
        p=new NODE<Create_PDR>;
        p->info.FAR_ID=FAR_ID;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address=ue2_ip;
        p->info.pdi.Network_Instance="VN_Group2";

        //FAR
        f=new NODE<Create_FAR>;
        f->info.FAR_ID=FAR_ID;
        FAR_ID++;
        f->info.Apply_Action="01";
        f->info.forwarding_parameters.Destination_Interface=1;//core side
        f->info.forwarding_parameters.Outer_Header_Creation.IP_Address=upf1_n19_ip;
        f->info.forwarding_parameters.Outer_Header_Creation.TEID=upf1_ue4_n19teid;
        group2_n4_est_req->create_pdr_all.Insert_Rear(p);
        group2_n4_est_req->create_far_all.Insert_Rear(f);
        */
        //4.增加能匹配UE3/UE4发出的广播数据包的PDR
        p=new NODE<Create_PDR>;
        p->info.PDR_ID=PDR_ID;
        PDR_ID++;
        p->info.FAR_ID=FAR_ID-2;//关联到只把包发送给对面UPF的FAR
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
        p->info.pdi.UE_IP_Address="255.255.255.255";
        p->info.pdi.Network_Instance="VN_Group2";
        group2_n4_est_req->create_pdr_all.Insert_Rear(p);

        qDebug()<<"[SMF] send Group-level N4 Session Establishment Request to UPF1 & UPF2..."<<endl;
        ui->Output_Window->append("[SMF] send Group-level N4 Session Establishment Request to UPF1 & UPF2...\n");
        delay();
        emit Group_Level_N4_Session_Est_Request_1(group1_n4_est_req);//发给UPF1的信号
        emit Group_Level_N4_Session_Est_Request_2(group2_n4_est_req);//发给UPF2的信号，似乎要等上一个信号调用的槽函数执行完后才发送
    }
}
