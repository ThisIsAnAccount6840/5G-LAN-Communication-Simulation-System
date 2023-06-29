#ifndef PDU_SESSION_EST_REQ_H
#define PDU_SESSION_EST_REQ_H

#include <QObject>
#include <QString>

#include "link_list.h"
#include "Request_Type.h"

class PDU_SESSION_EST_REQ : public QObject
{
    Q_OBJECT
public:
    explicit PDU_SESSION_EST_REQ(QObject *parent = nullptr);
    explicit PDU_SESSION_EST_REQ(QString, QString, int, int, LINK_LIST<QString> &);
    QString PDU_Session_ID;//SUPI+DNN
    QString requested_DNN;
    int req_sst;
    int req_sd;
    LINK_LIST<QString> Member_IDs;//UE想要进行通信的其他UE的member id
    QString UE_IP_Addre;//发起PDU会话的UE的IP地址
    bool in_Multicast_Group;//UE是否在组播组内
    REQ_TYPE req_type;
signals:

};

#endif // PDU_SESSION_EST_REQ_H
