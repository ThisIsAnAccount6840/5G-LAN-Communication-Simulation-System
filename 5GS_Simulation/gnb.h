#ifndef GNB_H
#define GNB_H

#include <QObject>
#include <QDebug>
#include "PDU_SESSION_EST_ACCEPT.h"
#include "pdu_session_related_info.h"
#include "link_list.h"
#include "interface.h"
#include "ui_mainwindow.h"

class gNB : public QObject
{
    Q_OBJECT
public:
    explicit gNB(QObject *parent, LINK_LIST<QString> &, QString, int, Ui::MainWindow *);//；最后一个参数：TEID初始值
    LINK_LIST<QString> UE_List;//所有连接到这个gNB的UE的SUPI链表
    int TEID_gNB;//N3接口在gNB一端的隧道端点标识符，从11or51开始
    LINK_LIST<int> gNB_TEID_List;
    LINK_LIST<PDU_Session_Related_Info> PDU_Info_List;//存有接收到的每一个PDU会话ID和对应的UPF分配的TEID
    QString gNB_IP_Address;
    gNB_N3 gNB_N3_Interface;
    Ui::MainWindow *ui;
    bool find_supi_in_ue_list(QString);
    //slot functions
    void handle_N2_PDUSession_Request(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);
    void AN_Specific_Resource_Setup_Recv();
signals:
    void AN_Specific_Resource_Setup(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);//第二个参数是UE的SUPI
    void N2_PDUSession_Response(int, QString);//第一个参数N3接口gNB端的TEID，第二个参数gNB的IP地址
};

#endif // GNB_H
