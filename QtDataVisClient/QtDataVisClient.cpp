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
	for(size_t idx = 0U; idx != m_figures.size(); ++idx)
	{
		//if (m_figures[idx] == nullptr)
		//{
		//    QMessageBox::information(this, tr("asdf"), tr("nullptr"), QMessageBox::Ok);
		//}
		//if(object == m_figures[idx])
		//{
		//	m_figures[idx]->deleteLater();
		//	//QMessageBox::information(this, tr("asdf"), tr("Found matching object... idx") + QString(idx), QMessageBox::Ok);
		//	//delete m_figures[idx];
		//	//m_figures[idx] = nullptr;
		//}
	}
}



void QtDataVisClient::OnPacket(const dvis::pkt::NetPacketT& packet)
{
	static int packet_count = 0;
	ui.m_packet_count->setText(QString::fromStdString("Packet count: " + std::to_string(++packet_count)));

	std::shared_ptr<dvis::Plot> plot;

	// Unpack it:
	if(packet.m_figure->m_xy_plot)
	{
		auto& rhs = packet.m_figure->m_xy_plot;
		plot = std::make_shared<dvis::XY_Plot>(
			rhs->m_series[0]->m_x,
			rhs->m_series[0]->m_y,
			rhs->m_x_label,
			rhs->m_y_label
			);
	}
	dvis::Figure fig
	(
		packet.m_figure->m_title,
		plot,
		packet.m_figure->m_ID
	);

	// Now we can see if there is a figure with that ID, else make a new figure
	auto figure_window = std::find_if(m_figures.begin(), m_figures.end(), [&packet](QPointer<QtFigure> fig) {return fig->ID() == packet.m_figure->m_ID;});

	if(figure_window != m_figures.end())
	{
		(*figure_window)->SetFigure(fig);
	}
	else
	{
		// Note: Will need to fix Widget must be created in the GUI thread. Probably rather push the packet onto a queue... .
		m_figures.push_back(QPointer<QtFigure>(new QtFigure(fig)));
	}
}


void QtDataVisClient::OnFigure()
{
	// This code can be moved to on create new figure
	m_figures.push_back(QPointer<QtFigure>(new QtFigure(dvis::Figure("Title", nullptr, -1))));
	m_figures.back()->show();
	m_figures.back()->connect(m_figures.back(), &QtFigure::FigureClosed, this, &QtDataVisClient::OnFigureClosed);

	// We can remove entries for figures that have since been deleted:
	m_figures.erase
	(
		std::remove_if
		(
			m_figures.begin(),
			m_figures.end(),
			[](QPointer<QtFigure> figure) {return figure == nullptr;}
		),
		m_figures.end()
	);
}

