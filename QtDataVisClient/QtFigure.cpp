/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#include "QtFigure.h"
#include <QMessageBox>

#include "qcustomplot.h"

QtFigure::QtFigure(dvis::Figure figure, QWidget* parent)
	: QMainWindow(parent),
	dvis::Figure(figure)
{
	ui.setupUi(this);
	//setWindowTitle()

	// generate some data:
	QVector<double> x(101), y(101); // initialize with entries 0..100
	for(int i = 0; i < 101; ++i)
	{
		x[i] = i / 50.0 - 1; // x goes from -1 to 1
		y[i] = x[i] * x[i]; // let's plot a quadratic function
	}
	// create graph and assign data to it:
	ui.m_plot->addGraph();
	ui.m_plot->graph(0)->setData(x, y);
	// give the axes some labels:
	ui.m_plot->xAxis->setLabel("x");
	ui.m_plot->yAxis->setLabel("y");
	// set axes ranges, so we see all data:
	ui.m_plot->xAxis->setRange(-1, 1);
	ui.m_plot->yAxis->setRange(0, 1);
	ui.m_plot->replot();
}



void QtFigure::SetFigure(const dvis::Figure& fig)
{
	// Might need to think about how bad this may be
	static_cast<dvis::Figure&>(*this) = fig;

	//Render(this);
	Draw();

	if(m_plot)
		m_plot->Draw(ui.m_plot);
}

void QtFigure::Draw()
{
	setWindowTitle(QString::fromStdString(Title()));

	if(ui.m_plot->graphCount() < 1)
		ui.m_plot->addGraph();

	ui.m_plot->setWindowTitle(QString::fromStdString(this->Title()));

	if(!m_title)
	{
		m_title = new QCPTextElement(ui.m_plot, QString::fromStdString(this->Title()), QFont("sans", 12, QFont::Bold));
		// add title layout element:
		ui.m_plot->plotLayout()->insertRow(0);
		ui.m_plot->plotLayout()->addElement(0, 0, m_title);
	}
	ui.m_plot->plotLayout()->addElement(0, 0, m_title);



	ui.m_plot->replot();
}

void QtXY_Plot::Draw(QCustomPlot* uiplot)
{
	uiplot->graph(0)->setData
	(
		QVector<qreal>::fromStdVector(m_x),
		QVector<qreal>::fromStdVector(m_y)
	);
	uiplot->xAxis->setLabel(QString::fromStdString(m_x_axis_bottom->m_label));
	uiplot->yAxis->setLabel(QString::fromStdString(m_y_axis_left->m_label));

	// Set ranges if specified, else auto scale:
	if(false)
	{
		// Fixed axis range
		uiplot->xAxis->setRange(m_x_axis_bottom->m_min, m_x_axis_bottom->m_max);
		uiplot->yAxis->setRange(m_y_axis_left->m_min, m_y_axis_left->m_max);
	}
	else
	{
		uiplot->rescaleAxes();
	}
}
