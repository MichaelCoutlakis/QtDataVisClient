/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once
#include <list>
#include <vector>
#include <memory>
#include <queue>
#include <mutex>

#include "../../NetDataVis/NetDataVis/NetBackendClient.h"

#include <QPointer>
#include <QtWidgets/QMainWindow>
#include "ui_QtDataVisClient.h"





class QtFigure;


/**
 * A client for the data visualization netowrk backend which will render to a Qt
 * backend.
 */
class QtDataVisClient : public QMainWindow
{
    Q_OBJECT

public:
    QtDataVisClient(QWidget *parent = Q_NULLPTR);
signals:
    /**
     * Signal to process the packets, necessary since packets arrive on a
     * worker thread but must be rendered on the GUI thread.
     */
    void ProcessPackets();
private slots:
    /**
     * Connect to the data visualization backend server.
     */
    void OnConnect();
    /**
     * Deal with a closed figure.
     * 
     * \param object
     */
    void OnFigureClosed(QObject* object);
    /**
     * Process received packets.
     * 
     */
    void OnProcessPackets();
private:
    /**
     * Callback for when a network packet is received.
     * 
     * \param packet
     */
    void OnPacket(std::shared_ptr<dvis::pkt::NetPacketT> packet);

    Ui::QtDataVisClientClass ui;

    /// Client to receive the network packets from the data visualization network backend
    std::shared_ptr<dvis::NetBackendClient> m_backend_client;
    /// All the figures which are plotted
    std::list<QPointer<QtFigure>> m_figures;

    /// Protect access to the packets and figures
    std::mutex m_packets_mutex;
    /// Buffer packets received on the socket thread for processing on the GUI thread
    std::queue<std::shared_ptr<dvis::pkt::NetPacketT>> m_packets;
};
