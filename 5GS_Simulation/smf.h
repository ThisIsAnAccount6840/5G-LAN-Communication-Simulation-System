#ifndef SMF_H
#define SMF_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "link_list.h"
#include "pdu_session_est_req.h"
#include "N4_Session_Establishment_Request.h"
#include "PDU_SESSION_EST_ACCEPT.h"
#include "mac_info.h"
#include "Time_Delay.h"
#include "ui_mainwindow.h"

class SMF : public QObject
{
    Q_OBJECT
public:
    explicit SMF(QObject *parent, LINK_LIST<QString> &, LINK_LIST<QString> &, Ui::MainWindow *);//第二、三个参数：UPF1 serve的所有UE的SUPI list和UPF2 serve的所有UE的SUPI list
    ~SMF();

    Ui::MainWindow *ui;

    LINK_LIST<QString> SMF_PDU_Session_ID;
    LINK_LIST<MAC_Info> *Member_MAC_Address_to_UE;//返回给UE的其他成员的IP地址
    QString UE_IP_Add;//当前正在建立PDU会话的UE的IP地址
    bool is_in_multicast_group;//当前正在进行PDU会话建立的UE是否在组播组内
    bool is_ue5_in_multicast_group;
    int group1_member_num;//VN组1成员数量
    LINK_LIST<QString> group1_member_supi_list;
    int group2_member_num;//VN组2成员数量
    LINK_LIST<QString> group2_member_supi_list;
    N4_SESSION_ESTABLISHMENT_REQUEST *est_req;
    PDU_SESSION_ESTABLISHMENT_ACCEPT pdu_session_est_accept;
    //Slot Functions
    void handle_CreateSMContext_Req(PDU_SESSION_EST_REQ *);
    void PDU_Session_Regis_Response_Recv(LINK_LIST<MAC_Info>*, QString);
    void handle_N4_Session_Est_Response(int);//参数是TEID
    void handle_UpdateSMContext_Req(int, QString);
    void handle_N4_Session_Modi_Response();
signals:
    void PDU_Session_Registration_Request(QString, LINK_LIST<QString> &);//参数1：PDU Session ID；参数2: UE想要通信的其他成员的ID
    void N4_Session_Est_Req(N4_SESSION_ESTABLISHMENT_REQUEST*);
    void N1N2_Message_Transfer_smf(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);//第二个参数是PDU Session ID
    void N4_Session_Modification_Request(int, QString, QString, QString);
    void Group_Level_N4_Session_Est_Request_1(N4_SESSION_ESTABLISHMENT_REQUEST *);
    void Group_Level_N4_Session_Est_Request_2(N4_SESSION_ESTABLISHMENT_REQUEST *);
    void PDU_Success_Show(QString);//参数是supi
};

#endif // SMF_H
