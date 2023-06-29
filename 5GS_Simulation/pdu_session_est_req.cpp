#include "pdu_session_est_req.h"

PDU_SESSION_EST_REQ::PDU_SESSION_EST_REQ(QObject *parent) : QObject(parent)
{

}

PDU_SESSION_EST_REQ::PDU_SESSION_EST_REQ(QString session_id, QString dnn, int sst, int sd, LINK_LIST<QString> &ids): QObject(nullptr){
    PDU_Session_ID=session_id;
    requested_DNN=dnn;
    req_sst=sst;
    req_sd=sd;
    Member_IDs=ids;
}
