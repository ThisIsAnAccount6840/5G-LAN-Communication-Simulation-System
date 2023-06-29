#include "gnb.h"

gNB::gNB(QObject *parent, LINK_LIST<QString> &ue_list, QString ip, int teid, Ui::MainWindow* u) : QObject(parent), gNB_N3_Interface(parent, ip, "gNB N3 Interface", u)
{
    ui=u;
    UE_List=ue_list;
    TEID_gNB=teid;
    gNB_IP_Address=ip;
    gNB_N3_Interface.Interface_TEID_List=&gNB_TEID_List;
    gNB_N3_Interface.PDU_Info_List=&PDU_Info_List;
}

bool gNB::find_supi_in_ue_list(QString s){
    return UE_List.find_node(s);
}

void gNB::handle_N2_PDUSession_Request(PDU_SESSION_ESTABLISHMENT_ACCEPT *acpt, QString sid){
    QString supi=sid.left(15);//12bit SUPI
    if(supi=="208950000000036"){ //通过N6连接的UE没连基站
        //qDebug()<<"**N6 UE"<<endl;
        emit AN_Specific_Resource_Setup(acpt, supi);
        return;
    }
    if(find_supi_in_ue_list(supi)!=true) return;
    qDebug()<<"[gNB] N2 PDU Session Request Received"<<endl;
    ui->Output_Window->append("[gNB] N2 PDU Session Request Received\n");
    TEID_gNB++;//每建立一个新的PDU会话就++
    NODE<int> *new_teid=new NODE<int>(TEID_gNB);
    gNB_TEID_List.Insert_Rear(new_teid);
    NODE<PDU_Session_Related_Info> *pdu_info=new NODE<PDU_Session_Related_Info>;
    pdu_info->info.PDU_Session_ID=sid;
    pdu_info->info.TEID=acpt->TEID;
    PDU_Info_List.Insert_Rear(pdu_info);
    qDebug()<<"AN Specific Signalling Exchange between UE and (R)AN"<<endl;
    ui->Output_Window->append("AN Specific Signalling Exchange between UE and (R)AN\n");
    emit AN_Specific_Resource_Setup(acpt, supi);
}

void gNB::AN_Specific_Resource_Setup_Recv(){
    qDebug()<<"[gNB] send N2 PDU Session Response..."<<endl;
    ui->Output_Window->append("[gNB] send N2 PDU Session Response...\n");
    emit N2_PDUSession_Response(TEID_gNB, gNB_IP_Address);
}
