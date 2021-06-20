/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/

#include "../../NetDataVis/NetDataVis/NetBackendClient.h"

#include "QtDataVisClient.h"
#include "QtFigure.h"
#include <QMessageBox>



QtDataVisClient::QtDataVisClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.m_connect, &QPushButton::clicked, this, &QtDataVisClient::OnConnect);
	connect(this, &QtDataVisClient::ProcessPackets, this, &QtDataVisClient::OnProcessPackets);
}


void QtDataVisClient::OnConnect()
{
	m_backend_client = std::make_shared<dvis::NetBackendClient>(
		ui.m_ip->text().toStdString(),
		ui.m_port->text().toUInt(),
		std::bind(&QtDataVisClient::OnPacket, this, std::placeholders::_1)
		);
}


void QtDataVisClient::OnFigureClosed(QObject* object)
{
	// If a figure is closed, we want to remove it from the list since we don't
	// know when it will be deleted and don't want another figure reploted when
	// it's invalid:
	std::lock_guard<std::mutex> lock(m_packets_mutex);

	for(auto iter = m_figures.begin(); iter != m_figures.end();)
	{
		if(object == *iter)
		{
			(*iter)->deleteLater();
			iter = m_figures.erase(iter);
		}
		else
			iter++;
	}
}

void QtDataVisClient::OnProcessPackets()
{
	std::lock_guard<std::mutex> lock(m_packets_mutex);
	static int packet_count = 0;
	ui.m_packet_count->setText(QString::fromStdString("Packet count: " + std::to_string(++packet_count)));

	std::shared_ptr<dvis::Plot> plot;

	auto& packet = m_packets.front();

	// Unpack it:
	if(packet->m_figure->m_xy_plot)
	{
		auto& rhs = packet->m_figure->m_xy_plot;
		plot = std::make_shared<dvis::XY_Plot>(
			rhs->m_series[0]->m_x,
			rhs->m_series[0]->m_y,
			rhs->m_x_label,
			rhs->m_y_label
			);
	}
	dvis::Figure fig
	(
		packet->m_figure->m_title,
		plot,
		packet->m_figure->m_ID
	);

	// Now we can see if there is a figure with that ID, else make a new figure
	auto figure_window = std::find_if
	(
		m_figures.begin(),
		m_figures.end(),
		[packet](QPointer<QtFigure> fig) {return fig->ID() == packet->m_figure->m_ID;}
	);

	if(figure_window != m_figures.end())
	{
		(*figure_window)->SetFigure(fig);
	}
	else
	{
		m_figures.push_back(QPointer<QtFigure>(new QtFigure(fig)));
		m_figures.back()->connect(m_figures.back(), &QtFigure::FigureClosed, this, &QtDataVisClient::OnFigureClosed);
		m_figures.back()->show();
	}
	m_packets.pop();
}



void QtDataVisClient::OnPacket(std::shared_ptr<dvis::pkt::NetPacketT> packet)
{
	// Queue the packet for processing on the UI thread
	std::lock_guard<std::mutex> lock(m_packets_mutex);
	m_packets.push(packet);
	emit ProcessPackets();
}
