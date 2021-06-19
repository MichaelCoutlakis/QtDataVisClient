/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#include "QtDataVisClient.h"
#include "QtFigure.h"
#include <QMessageBox>

QtDataVisClient::QtDataVisClient(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, &QPushButton::clicked, this, &QtDataVisClient::OnConnect);
}


void QtDataVisClient::OnConnect()
{
	// This code can be moved to on create new figure
	m_figures.push_back(QPointer<QtFigure>(new QtFigure()));
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


void QtDataVisClient::OnFigureClosed(QObject* object)
{
	for (size_t idx = 0U; idx != m_figures.size(); ++idx)
	{
		//if (m_figures[idx] == nullptr)
		//{
		//    QMessageBox::information(this, tr("asdf"), tr("nullptr"), QMessageBox::Ok);
		//}
		if (object == m_figures[idx])
		{
			m_figures[idx]->deleteLater();
			//QMessageBox::information(this, tr("asdf"), tr("Found matching object... idx") + QString(idx), QMessageBox::Ok);
			//delete m_figures[idx];
			//m_figures[idx] = nullptr;
		}
	}
}