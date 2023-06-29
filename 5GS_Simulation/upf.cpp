#include "upf.h"

UPF::UPF(QObject *parent, QString group_name,int teid, QString n3_ip, QString n19_ip, QString n3_name, QString n6_name, QString n19_name, Ui::MainWindow *ui_p) : QObject(parent), UPF_N3_Interface(nullptr, n3_ip, n3_name, ui_p), UPF_N6_Interface(nullptr, "null", n6_name, ui_p),UPF_N19_Interface(nullptr, n19_ip, n19_name, ui_p)
{
    UPF_N3_Interface_IP=n3_ip;
    UPF_N3_Interface.Interface_TEID_List=&UPF_TEID_List;
    serving_group_name=group_name;
    TEID_UPF=teid;
    ui=ui_p;
}

void UPF::handle_N4_Session_Est_Req(N4_SESSION_ESTABLISHMENT_REQUEST * est_req){//建立本地N4会话
    if(est_req->create_pdr_all.head->link->info.pdi.UE_IP_Address=="12.1.1.3"){
        if(serving_group_name=="VN_Group2") return;//UPF2若收到UE2的PDU会话建立请求则丢弃
    }
    else if(est_req->create_pdr_all.head->link->info.pdi.Network_Instance!=serving_group_name){
        return;
    }
    qDebug()<<"[UPF] N4 Session Establishment Request Received"<<endl;
    ui->Output_Window->append("[UPF] N4 Session Establishment Request Received\n");
    if(est_req->create_pdr_all.head->link->info.pdi.UE_IP_Address=="12.1.1.6"){//UE5发来的请求，不可建立本地N4会话
        qDebug()<<"[UPF] send N4 Session Establishment Response..."<<endl;
        ui->Output_Window->append("[UPF] send N4 Session Establishment Response...\n");
        delay();
        //delete est_req;
        emit N4_Session_Estblishment_Response(TEID_UPF);
        return;
    }
    NODE<N4_SESSION_CONTEXT> *ue_n4_context=new NODE<N4_SESSION_CONTEXT>;
    N4_Session_Context_List.Insert_Rear(ue_n4_context);
    ue_n4_context->info.UE_IP=est_req->create_pdr_all.head->link->info.pdi.UE_IP_Address;//当前正在进行PDU会话建立的UE的IP地址
    ue_n4_context->info.FAR_LIST=est_req->create_far_all;
    ue_n4_context->info.PDR_LIST=est_req->create_pdr_all;
    NODE<Create_FAR> *f;
    NODE<Create_PDR> *p;
    int PDR_ID, FAR_ID;
    QString nw_inst;
    p=ue_n4_context->info.PDR_LIST.head->link;
    nw_inst=p->info.pdi.Network_Instance;
    //1.Local Switch上行PDR剩余参数的配置(单播，广播，组播)：
    p->info.pdi.Source_Interface=0;//access;
    p->info.pdi.Local_FTEID.IP_Address=UPF_N3_Interface_IP;
    p->info.pdi.Local_FTEID.TEID=++TEID_UPF;
    NODE<int> *new_teid=new NODE<int>(TEID_UPF);
    UPF_TEID_List.Insert_Rear(new_teid);
    //Local Switch上行PDR关联的FAR的配置(单播,广播，组播)：
    f=ue_n4_context->info.find_FAR_with_ID(p->info.FAR_ID);
    if(f==nullptr){
        qDebug()<<"ERROR: FAR NOT FOUND!"<<endl;
        ui->Output_Window->append("ERROR: FAR NOT FOUND!\n");
        return;
    }
    f->info.Apply_Action="01";//DUPL=0, FORW=1
    f->info.forwarding_parameters.Destination_Interface=5;//5G VN Internal

    //2.Local Switch单播下行PDR的PDI的配置（缺F-TEID，需等到gNB分配了TEID后才能配置，即N4 session update那一步配置）
    p=p->link;
    p->info.pdi.Source_Interface=4;//5G VN Internal
    PDR_ID=p->info.PDR_ID;
    //Local Switch单播下行PDR对应的FAR的配置(同样缺F-TEID)：
    f=ue_n4_context->info.find_FAR_with_ID(p->info.FAR_ID);
    if(f==nullptr){
        qDebug()<<"ERROR: FAR NOT FOUND!"<<endl;
        ui->Output_Window->append("ERROR: FAR NOT FOUND!\n");
        return;
    }
    f->info.Apply_Action="01";
    f->info.forwarding_parameters.Destination_Interface=0;//access side
    FAR_ID=f->info.FAR_ID;

    //3.广播下行PDR&FAR的配置
    p=new NODE<Create_PDR>;
    p->info.PDR_ID=++PDR_ID;
    p->info.FAR_ID=++FAR_ID;
    p->info.pdi.Source_Interface=4;//5G VN Internal
    p->info.pdi.UE_IP_Address="255.255.255.255";//broadcast ip address
    p->info.pdi.Network_Instance=nw_inst;
    ue_n4_context->info.PDR_LIST.Insert_Rear(p);
    //FAR--缺F-TEID
    f=new NODE<Create_FAR>;
    f->info.FAR_ID=p->info.FAR_ID;
    f->info.Apply_Action="11";//DUPL&FORWD
    f->info.duplicating_parameters.Destination_Interface=0;//access side
    ue_n4_context->info.FAR_LIST.Insert_Rear(f);

    //(4)组播下行PDR&FAR
    if(est_req->is_in_multi==true){//只有当UE在组播组内时才配置该PDR和FAR
        p=new NODE<Create_PDR>;
        p->info.PDR_ID=++PDR_ID;
        p->info.FAR_ID=FAR_ID;//关联到广播下行的FAR
        p->info.pdi.Source_Interface=4;//5G VN Internal
        p->info.pdi.UE_IP_Address="244.1.1.1";//multicast ip address
        p->info.pdi.Network_Instance=nw_inst;
        ue_n4_context->info.PDR_LIST.Insert_Rear(p);
        //FAR--使用广播下行的FAR
    }

    qDebug()<<"[UPF] send N4 Session Establishment Response..."<<endl;
    ui->Output_Window->append("[UPF] send N4 Session Establishment Response...\n");
    delay();
    //qDebug()<<"为正在进行PDU会话建立的UE创建的N4会话内容："<<endl;
    //ue_n4_context->info.print_N4SessionContext();
    //delete est_req;
    emit N4_Session_Estblishment_Response(TEID_UPF);
}

void UPF::handle_N4_Session_Modi_Req(int gnb_teid, QString gnb_ip, QString group_name, QString supi){
    if(supi=="208950000000033"){//UE2
        if(serving_group_name=="VN_Group2") return;
    }
    else if(group_name!=serving_group_name) return;
    qDebug()<<"[UPF] N4 Session Modification Request Received"<<endl;
    ui->Output_Window->append("[UPF] N4 Session Modification Request Received\n");
    NODE<Create_FAR> *f;
    NODE<Create_PDR> *p;
    //if(N4_Session_Context_List.tail->info.PDR_LIST.head->link==nullptr) qDebug()<<"***"<<endl;
    p=N4_Session_Context_List.tail->info.PDR_LIST.head->link->link;//指向local switch下行PDR
    //Local Switch单播下行PDR的PDI的剩余参数的配置(即F-TEID配置为all-match)
    p->info.pdi.Local_FTEID.IP_Address="null";
    p->info.pdi.Local_FTEID.TEID=-1;
    //Local Switch单播下行PDR对应的FAR的配置(即F-TEID的配置)：
    f=N4_Session_Context_List.tail->info.find_FAR_with_ID(p->info.FAR_ID);
    if(f==nullptr){
        qDebug()<<"ERROR: FAR NOT FOUND!"<<endl;
        ui->Output_Window->append("ERROR: FAR NOT FOUND!\n");
        return;
    }
    f->info.forwarding_parameters.Outer_Header_Creation.TEID=gnb_teid;
    f->info.forwarding_parameters.Outer_Header_Creation.IP_Address=gnb_ip;

    //广播下行PDR&FAR的配置：
    p=p->link;
    p->info.pdi.Local_FTEID.IP_Address="null";
    p->info.pdi.Local_FTEID.TEID=-1;
    //FAR
    f=N4_Session_Context_List.tail->info.find_FAR_with_ID(p->info.FAR_ID);
    if(f==nullptr){
        qDebug()<<"ERROR: FAR NOT FOUND!"<<endl;
        ui->Output_Window->append("ERROR: FAR NOT FOUND!\n");
        return;
    }
    f->info.duplicating_parameters.Outer_Header_Creation.TEID=gnb_teid;
    f->info.duplicating_parameters.Outer_Header_Creation.IP_Address=gnb_ip;

    if(p->link!=nullptr){//组播下行PDR剩余参数的配置
        p=p->link;
        p->info.pdi.Local_FTEID.IP_Address="null";
        p->info.pdi.Local_FTEID.TEID=-1;
    }

    qDebug()<<"====PRINT the FINAL N4 Session Context===="<<endl;
    N4_Session_Context_List.tail->info.print_N4SessionContext();

    qDebug()<<"[UPF] send N4 Session Modification Response..."<<endl;
    ui->Output_Window->append("[UPF] send N4 Session Modification Response...\n");
    delay();
    emit N4_Session_Modification_Response();
}

void UPF::handle_Group_Level_N4_Seesion_Est_Request(N4_SESSION_ESTABLISHMENT_REQUEST * est_req){
    if(serving_group_name=="VN_Group1") {
        qDebug()<<"[UPF1] Group-level N4 Session Establishment Request Received";
        ui->Output_Window->append("[UPF1] Group-level N4 Session Establishment Request Received\n");
    }
    else {
        qDebug()<<"[UPF2] Group-level N4 Session Establishment Request Received";
        ui->Output_Window->append("[UPF2] Group-level N4 Session Establishment Request Received\n");
    }
    NODE<N4_SESSION_CONTEXT> *group_n4_session_ctx=new NODE<N4_SESSION_CONTEXT>;
    group_n4_session_ctx->info.UE_IP="12.1.1.6";//只在UPF1内使用
    group_n4_session_ctx->info.FAR_LIST=est_req->create_far_all;
    group_n4_session_ctx->info.PDR_LIST=est_req->create_pdr_all;
    N4_Session_Context_List.Insert_Rear(group_n4_session_ctx);
    qDebug()<<"***PRINT GROUP-LEVEL N4 SESSION CONTEXT***"<<endl;
    N4_Session_Context_List.tail->info.print_N4SessionContext();
    //delete est_req;
    qDebug()<<"[UPF] send Group-level N4 Session Establishment Response..."<<endl;
    ui->Output_Window->append("[UPF] send Group-level N4 Session Establishment Response...\n");
    delay();
    qDebug()<<"[SMF] Group-level N4 Session Establishment Response Received"<<endl;
    ui->Output_Window->append("[SMF] Group-level N4 Session Establishment Response Received\n");
    qDebug()<<"=====Group-level N4 Session Configuration Complete!====="<<endl;
    ui->Output_Window->append("=====Group-level N4 Session Configuration Complete!=====\n");
}

void UPF::find_PFCP_Session_with_a_matching_PDR(Data_Packet &data, QString ip){
    PDI ue_PDI;
    ue_PDI.Source_Interface=data.src_interface;
    ue_PDI.Local_FTEID.IP_Address=ip;
    ue_PDI.Local_FTEID.TEID=data.TEID;
    if(data.src_interface==4){//5G VN Internal, downlink data packet
        ue_PDI.UE_IP_Address=data.dst_ip;
    }
    else ue_PDI.UE_IP_Address=data.src_ip;//uplink data packet
    ue_PDI.Network_Instance=data.network_instance;

    NODE<N4_SESSION_CONTEXT> *n4_ctx;
    Create_PDR* match_pdr=nullptr;
    Create_FAR* match_far=nullptr;
    bool result;
    for(n4_ctx=N4_Session_Context_List.head->link; n4_ctx!=nullptr; n4_ctx=n4_ctx->link){
        //qDebug()<<"====Print Current N4 Session Context===="<<endl;
        //n4_ctx->info.print_N4SessionContext();
        match_pdr=n4_ctx->info.find_matching_pdr_in_N4SessionContext(ue_PDI);
        if(match_pdr!=nullptr) {
            //found a matching pdr and execute the corresponding far
            //qDebug()<<"***FOUND MATCHING PDR"<<endl;
            //match_pdr->print_PDR();
            match_far=&((n4_ctx->info.find_FAR_with_ID(match_pdr->FAR_ID))->info);//在当前N4 Session Context中使用匹配的PDR找到对应的FAR
            //qDebug()<<"****MATCH FAR:"<<endl;
            //match_far->print_far();
            result=execute_FAR(data, match_far, match_pdr, n4_ctx);
            if(result==false) return;
        }
    }
    if(match_pdr==nullptr&&n4_ctx!=nullptr){
        qDebug()<<"ERROR: NO MATCHING PDR!"<<endl;
        ui->Output_Window->append("ERROR: NO MATCHING PDR!\n");
        return;
    }
    delete &data;//原始数据包的释放
}

bool UPF::execute_FAR(Data_Packet &data, Create_FAR *match_far, Create_PDR* match_pdr, NODE<N4_SESSION_CONTEXT> * n4_ctx){
    if(match_far->Apply_Action=="01"){//DUPL:0; FOWRD:1
        if(match_far->forwarding_parameters.Destination_Interface==5){//5G VN Internal
            qDebug()<<"[UPF] send a packet to 5G VN Internal interface"<<endl;
            ui->Output_Window->append("[UPF] send a packet to 5G VN Internal interface\n");
            delay();
            _5G_VN_Internal_Recv(data);
            return 0;
        }
        if(match_far->forwarding_parameters.Destination_Interface==0){//access side, local switch downlink
            data.TEID=match_far->forwarding_parameters.Outer_Header_Creation.TEID;
            qDebug()<<"[UPF] send a packet to N3 interface... "<<endl;
            ui->Output_Window->append("[UPF] send a packet to N3 interface...\n");
            //data.print_data_packet();
            delay();
            emit send_to_N3_Interface(data);
            return 0;
        }
        if(match_far->forwarding_parameters.Destination_Interface==1){//core side, N6/N19
            if(match_far->forwarding_parameters.Outer_Header_Creation.IP_Address=="null" || match_far->forwarding_parameters.Outer_Header_Creation.TEID==-1){//N6 based
                data.TEID=-1;
                qDebug()<<"[UPF] send a packet to N6 interface..."<<endl;
                ui->Output_Window->append("[UPF] send a packet to N6 interface...\n");
                delay();
                emit send_to_N6_Interface(data, "");
                return 0;
            }
            else{//N19
                if(match_pdr->rep_detc_info.PRIN19I==true){
                    match_pdr->rep_detc_info.PRIN19I=false;
                    return true;//广播PDR关联的是正常转发的FAR
                }
                data.TEID=match_far->forwarding_parameters.Outer_Header_Creation.TEID;
                qDebug()<<"[UPF] send a packet to N19 interface..."<<endl;
                ui->Output_Window->append("[UPF] send a packet to N19 interface...\n");
                delay();
                emit send_to_N19_Interface(data);
                return 0;
            }
        }
    }
    else if(match_far->Apply_Action=="11"){//DUPL:1,FOWRD:1
        if(match_pdr->rep_detc_info.PRIUEAI==true || match_pdr->rep_detc_info.PRIN6I==true || match_pdr->rep_detc_info.PRIN19I==true){//从哪一个接口收到了广播数据包，哪个参数就是true
            if(match_pdr->rep_detc_info.PRIUEAI==true) {
                //qDebug()<<"PRIUEAI=TRUE"<<endl;
                match_pdr->rep_detc_info.PRIUEAI=false;
            }
            if(match_pdr->rep_detc_info.PRIN6I==true) {
                //qDebug()<<"PRIN6I==TRUE"<<endl;
                match_pdr->rep_detc_info.PRIN6I=false;
            }
            if(match_pdr->rep_detc_info.PRIN19I==true) {
                //qDebug()<<"PRIN19I==TRUE"<<endl;
                match_pdr->rep_detc_info.PRIN19I=false;
            }
            return match_pdr->rep_detc_info.DCARONI;//(实际上就是true) 指示UPF继续进行PDR的匹配
        }
        else{//复制数据包并转发
            Data_Packet *uni_packet=new Data_Packet;
            uni_packet->src_ip=data.src_ip;
            uni_packet->dst_ip=n4_ctx->info.UE_IP;
            uni_packet->src_interface=data.src_interface;
            uni_packet->TEID=match_far->duplicating_parameters.Outer_Header_Creation.TEID;
            uni_packet->T_PDU=data.T_PDU;
            uni_packet->network_instance=data.network_instance;
            //复制完成后的转发只会通过N3或N6
            if(match_far->duplicating_parameters.Destination_Interface==0){//access side
                qDebug()<<"[UPF] send a packet to N3 interface... "<<endl;
                ui->Output_Window->append("[UPF] send a packet to N3 interface...\n");
                //uni_packet->print_data_packet();
                delay();
                emit send_to_N3_Interface(*uni_packet);//数据包发送到UE后才会回到这个语句，才能进行下一个UE数据包的复制
                return true;//继续寻找下一个匹配的PDR
            }
            else{//N6 based
                uni_packet->TEID=-1;
                qDebug()<<"[UPF] send a packet to N6 interface..."<<endl;
                ui->Output_Window->append("[UPF] send a packet to N6 interface...\n");
                delay();
                emit send_to_N6_Interface(*uni_packet, "");
                return true;//继续寻找下一个匹配的PDR
            }
            /*else{//N19的复制在本地进行
                uni_packet->TEID=match_far->duplicating_parameters.Outer_Header_Creation.TEID;
                uni_packet->dst_ip=match_pdr->pdi.Local_FTEID.IP_Address;//UE3 or UE4的地址
                qDebug()<<"[UPF] send a packet to N19 interface..."<<endl;
                emit send_to_N19_Interface(*uni_packet);
                return true;//继续寻找下一个匹配的PDR
            }*/
        }
    }
    else return 0;
}

void UPF::_5G_VN_Internal_Recv(Data_Packet &data){
    qDebug()<<"5G VN Internal received a packet"<<endl;
    ui->Output_Window->append("5G VN Internal received a packet\n");
    data.src_interface=4;//5G VN Internal
    data.TEID=-1;
    qDebug()<<"5G VN Internal send a packet..."<<endl;
    ui->Output_Window->append("5G VN Internal send a packet...\n");
    //data.print_data_packet();
    find_PFCP_Session_with_a_matching_PDR(data,"null");//ip="null" in default
}

void UPF::handle_broadcast_packet(QString ue_ip, int interface, QString dst_ip, QString nw_ins){
    NODE<N4_SESSION_CONTEXT> *n4_ctx;
    NODE<Create_PDR> *p;
    for(n4_ctx=N4_Session_Context_List.head->link;n4_ctx!=nullptr;n4_ctx=n4_ctx->link){
        if(n4_ctx->info.UE_IP==ue_ip && n4_ctx->info.PDR_LIST.head->link->info.pdi.Network_Instance==nw_ins){
            for(p=n4_ctx->info.PDR_LIST.head->link;p!=nullptr;p=p->link){
                if(p->info.pdi.UE_IP_Address==dst_ip){
                    if(interface==3) p->info.rep_detc_info.PRIUEAI=true;
                    if(interface==6) p->info.rep_detc_info.PRIN6I=true;
                    //if(interface==19) p->info.rep_detc_info.PRIN19I=true;
                    break;
                }
            }
            break;
        }
    }
    if(n4_ctx==nullptr){//没有找到匹配的UE IP，说明是接收到了另一个UPF发来的广播数据包
        for(n4_ctx=N4_Session_Context_List.head->link;n4_ctx!=nullptr;n4_ctx=n4_ctx->link){
            if(n4_ctx->info.UE_IP=="12.1.1.6" && n4_ctx->info.PDR_LIST.head->link->info.pdi.Network_Instance==nw_ins){//找到对应组的组级N4会话
                for(p=n4_ctx->info.PDR_LIST.head->link;p!=nullptr;p=p->link){
                    if(p->info.pdi.UE_IP_Address=="255.255.255.255"){
                        p->info.rep_detc_info.PRIN19I=true;
                        break;
                    }
                }
                break;
            }
        }
    }
}
