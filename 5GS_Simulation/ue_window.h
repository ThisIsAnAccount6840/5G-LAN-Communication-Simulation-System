#ifndef UE_WINDOW_H
#define UE_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QString>

#include "pdu_window.h"
#include "sendpacket_window.h"
#include "error_window.h"

class UE_Window : public QWidget
{
    Q_OBJECT
public:
    explicit UE_Window(QWidget *parent = nullptr, QString name="UE");
    QPushButton Register;
    QPushButton PDU_Est;
    QPushButton Start;
    PDU_Window PDU_win;
    SendPacket_Window send_packet_window;
    ERROR_WINDOW error_win;
    ERROR_WINDOW multi_error_win;

    //slot functions:
    void UE_Window_Show();
signals:

};

#endif // UE_WINDOW_H
