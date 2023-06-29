#ifndef INTERFACE_H
#define INTERFACE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

#include "data_packet.h"
#include "link_list.h"
#include "PDU_SESSION_EST_ACCEPT.h"
#include "pdu_session_related_info.h"
#include "Time_Delay.h"
#include "ui_mainwindow.h"

class Interface : public QObject
{
    Q_OBJECT
public:
    explicit Interface(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow* = nullptr);//第二个参数：IP地址；第三个参数：接口名
    Ui::MainWindow *ui;
    QString Interface_IP;
    QString Interface_Name;
    LINK_LIST<int> *Interface_TEID_List;//存有该接口所属类的TEID链表地址

    void send_to_CN(Data_Packet &, QString);//第二个参数：接口IP地址
signals:
    void broadcast_packet_receive(QString, int, QString, QString);//第一个参数是发包的UE IP地址，第二个参数是接口编号，第三个参数是目的IP地址，第四个参数是network instance
};

class gNB_N3: public Interface
{
    Q_OBJECT
public:
    gNB_N3(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow * =nullptr);
    LINK_LIST<PDU_Session_Related_Info> *PDU_Info_List;
    void Recev_packet(Data_Packet &, QString);//第二个参数：PDU会话ID
signals:
    void uplink_data(Data_Packet &);
    void downlink_data(Data_Packet &);
};

class UPF_N3: public Interface
{
    Q_OBJECT
public:
    UPF_N3(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow * =nullptr);
    void Recev_packet(Data_Packet &);
signals:
    void uplink_data(Data_Packet &, QString);
    void downlink_data(Data_Packet &, QString);
};

class UPF_N6: public Interface
{
    Q_OBJECT
public:
    UPF_N6(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow * =nullptr);
    void Recev_packet(Data_Packet &, QString);
signals:
    void uplink_data(Data_Packet &, QString ="null");
    void downlink_data(Data_Packet &);
};

class UPF_N19: public Interface
{
    Q_OBJECT
public:
    UPF_N19(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow * = nullptr);
    void Recev_packet(Data_Packet &);
signals:
    void send_data(Data_Packet &);
    void recv_data(Data_Packet &, QString);
};

class _5G_VN_INTERNAL: public Interface
{
    Q_OBJECT
public:
    _5G_VN_INTERNAL(QObject *parent = nullptr, QString ="null", QString ="", Ui::MainWindow * = nullptr);
    void Recev_packet(Data_Packet &);
signals:
    void uplink_data(Data_Packet &);
    void downlink_data(Data_Packet &);
};


#endif // INTERFACE_H
