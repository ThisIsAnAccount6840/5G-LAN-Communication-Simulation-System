#include "amf.h"

AMF::AMF(QObject *parent, Ui::MainWindow *ui_p):QObject(parent)
{
    regis_acpt=nullptr;
    regis_reqst=nullptr;
    ui=ui_p;
}

AMF::~AMF(){
    delete regis_acpt;
    //delete regis_reqst;
}

void AMF::handle_Regis_Req(REGISTRATION_REQUEST * Regis_Req_Msg){
    qDebug()<<"[AMF] Registration Request Received"<<endl;
    ui->Output_Window->append("[AMF] Registration Request Received\n");
    //delete regis_reqst;
    //regis_reqst=new REGISTRATION_REQUEST(*Regis_Req_Msg);
    regis_reqst=Regis_Req_Msg;
    regis_reqst->Group_Member_ID.print();

    //TODO: UE Authentication Procedure

    qDebug()<<"[AMF] Get UE Subscription Data from UDM..."<<endl;
    ui->Output_Window->append("[AMF] Get UE Subscription Data from UDM...\n");
    delay();
    emit UE_Subscrib_Data_Get(Regis_Req_Msg->supi);
}

void AMF::recev_ue_sub_data(UE_Sub_Data *u_s){
    if(u_s->LAN_CAP==true && regis_reqst->LAN_Capability==true){
        NODE<QString> *temp;
        for(temp=regis_reqst->Group_Member_ID.head->link;temp!=nullptr;temp=temp->link){
            if(u_s->LAN_Member_ID.find_node(temp->info)==nullptr){
                qDebug()<<"ERROR: Group member ID Discrepancy"<<endl;
                ui->Output_Window->append("ERROR: Group member ID Discrepancy\n");
                return;
            }
        }//LAN Member ID验证结束
        qDebug()<<"[AMF] Send Group Authentication Request to GMF..."<<endl;
        ui->Output_Window->append("[AMF] Send Group Authentication Request to GMF...\n");
        delay();
        //把UE能访问的所有LAN组名以及UE在组里面使用的ID一起发给GMF验证，如果验证通过，就注册IP address到对应链表中
        emit UE_Group_Registration_Request(u_s->LAN_num, regis_reqst->supi, u_s->LAN_Member_ID, u_s->LAN_Group_Name, regis_reqst->MAC_addr);
    }
    else{
        qDebug()<<"ERROR: 5GLAN NOT SUPPORTED"<<endl;
        ui->Output_Window->append("ERROR: 5GLAN NOT SUPPORTED\n");
    }
}

void AMF::handle_group_regis_resp(bool result, LAN_INFO* *lan_config, LINK_LIST<QString>*member_ids){
    if(result==false){
        qDebug()<<"ERROR: Group Authentication Failure"<<endl;
        ui->Output_Window->append("ERROR: Group Authentication Failure\n");
        return;
    }
    else{
        //regis_acpt=new REGISTRATION_ACCEPT(group_name, id, dnn, sst, sd);
        qDebug()<<"[AMF] Group Registration Response Received"<<endl;
        ui->Output_Window->append("[AMF] Group Registration Response Received\n");
        qDebug()<<"[AMF] send Registration Accept Message..."<<endl;
        ui->Output_Window->append("[AMF] send Registration Accept Message...\n");
        delay();
        emit Registration_Accept_Msg(regis_reqst->supi, lan_config, member_ids);
    }
}

void AMF::handle_PDU_Session_Est_Req(PDU_SESSION_EST_REQ *est){
    qDebug()<<"[AMF] PDU Session Establishment Request Received"<<endl;
    ui->Output_Window->append("[AMF] PDU Session Establishment Request Received\n");
    qDebug()<<"[AMF] send PDUSession_CreateSMContext_Request..."<<endl;
    ui->Output_Window->append("[AMF] send PDUSession_CreateSMContext_Request...\n");
    delay();
    emit PDUSession_CreateSMContext_Request(est);
}

void AMF::N1N2_Message_Transfer_amf(PDU_SESSION_ESTABLISHMENT_ACCEPT * acpt, QString sid){
    QString supi=sid.left(15);
    if(supi!="208950000000036") {
        qDebug()<<"[AMF] send N2 PDU Session Request..."<<endl;
        ui->Output_Window->append("[AMF] send N2 PDU Session Request...\n");
    }
    emit N2_PDU_Session_Request(acpt, sid);
}

void AMF::handle_N2_PDUSession_Response(int teid_gnb, QString gnb_ip){
    qDebug()<<"[AMF] N2 PDU Session Response Received"<<endl;
    ui->Output_Window->append("[AMF] N2 PDU Session Response Received\n");
    qDebug()<<"=====UPLINK CONFIGURATION COMPLETE====="<<endl;
    ui->Output_Window->append("=====UPLINK CONFIGURATION COMPLETE=====\n");
    qDebug()<<"[AMF] send PDUSession UpdateSMContext Request..."<<endl;
    ui->Output_Window->append("[AMF] send PDUSession UpdateSMContext Request...\n");
    emit PDUSession_UpdateSMContext_Request(teid_gnb, gnb_ip);
}
