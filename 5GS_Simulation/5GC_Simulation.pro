QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    IEs.cpp \
    N4_Session_Context.cpp \
    N4_Session_Establishment_Request.cpp \
    Time_Delay.cpp \
    UE_Sub_Data.cpp \
    amf.cpp \
    data_packet.cpp \
    error_window.cpp \
    gmf.cpp \
    gnb.cpp \
    interface.cpp \
    lan_info.cpp \
    mac_info.cpp \
    main.cpp \
    mainwindow.cpp \
    pdu_session_est_req.cpp \
    pdu_window.cpp \
    registration_accept.cpp \
    registration_request.cpp \
    sendpacket_window.cpp \
    smf.cpp \
    udm.cpp \
    ue.cpp \
    ue_info.cpp \
    ue_sub_data_node.cpp \
    ue_window.cpp \
    upf.cpp

HEADERS += \
    IEs.h \
    MM_State.h \
    N4_Session_Context.h \
    N4_Session_Establishment_Request.h \
    PDU_SESSION_EST_ACCEPT.h \
    Request_Type.h \
    Time_Delay.h \
    UE_Sub_Data.h \
    amf.h \
    data_packet.h \
    error_window.h \
    gmf.h \
    gnb.h \
    interface.h \
    lan_info.h \
    link_list.h \
    mac_info.h \
    mainwindow.h \
    pdu_session_est_req.h \
    pdu_session_related_info.h \
    pdu_window.h \
    registration_accept.h \
    registration_request.h \
    sendpacket_window.h \
    smf.h \
    udm.h \
    ue.h \
    ue_info.h \
    ue_sub_data_node.h \
    ue_window.h \
    upf.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    gNB.jpeg

RESOURCES += \
    gNB_Image.qrc
