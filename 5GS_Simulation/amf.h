#ifndef AMF_H
#define AMF_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTime>
#include <QCoreApplication>

#include "registration_request.h"
#include "registration_accept.h"
#include "pdu_session_est_req.h"
#include "PDU_SESSION_EST_ACCEPT.h"
#include "UE_Sub_Data.h"
#include "ue_info.h"
#include "link_list.h"
#include "Time_Delay.h"
#include "ui_mainwindow.h"

class AMF:public QObject
{
    Q_OBJECT
public:
    explicit AMF(QObject *parent = nullptr, Ui::MainWindow *ui=nullptr);
    ~AMF();

    Ui::MainWindow *ui;

    //Slot Functions
    void handle_Regis_Req(REGISTRATION_REQUEST *);
    void recev_ue_sub_data(UE_Sub_Data*);
    void handle_group_regis_resp(bool, LAN_INFO**, LINK_LIST<QString> *);
    void handle_PDU_Session_Est_Req(PDU_SESSION_EST_REQ *);
    void N1N2_Message_Transfer_amf(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);
    void handle_N2_PDUSession_Response(int, QString);//第一个参数gNB端分配的TEID，第二个参数gNB的IP地址
private:
    REGISTRATION_ACCEPT *regis_acpt;
    REGISTRATION_REQUEST *regis_reqst;
signals:
    void UE_Subscrib_Data_Get(QString);
    void UE_Group_Registration_Request(int, QString, LINK_LIST<QString>, LINK_LIST<QString>, QString);//参数1：需要验证的ID及对应的组数量
                                                                                                     //参数2：UE的SUPI；参数3：要验证的IDs；参数4：上述ID对应的LAN组名；参数5：UE的MAC地址
    void N2_PDU_Session_Request(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);
    void Registration_Accept_Msg(QString, LAN_INFO **, LINK_LIST<QString>*);
    void PDUSession_CreateSMContext_Request(PDU_SESSION_EST_REQ *);
    void PDUSession_UpdateSMContext_Request(int, QString);//第一个参数gnb分配的TEID，第二个参数gNB的IP地址
};

#endif // AMF_H
