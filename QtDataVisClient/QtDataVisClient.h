/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once
#include <vector>
#include <memory>

#include "../../NetDataVis/NetDataVis/NetBackendClient.h"

#include <QPointer>
#include <QtWidgets/QMainWindow>
#include "ui_QtDataVisClient.h"





class QtFigure;



class QtDataVisClient : public QMainWindow
{
    Q_OBJECT

public:
    QtDataVisClient(QWidget *parent = Q_NULLPTR);
private slots:
    void OnConnect();
    void OnFigureClosed(QObject* object);
private:

    void OnPacket(const dvis::pkt::NetPacketT& packet);


    void OnFigure();


    Ui::QtDataVisClientClass ui;
    std::shared_ptr<dvis::NetBackendClient> m_backend_client;

    std::vector<QPointer<QtFigure>> m_figures;
};
