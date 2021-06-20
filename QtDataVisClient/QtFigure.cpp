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

	Render(this);
}

void QtFigure::RenderFigure(Figure* fig)
{
	setWindowTitle(QString::fromStdString(Title()));
}

void QtFigure::RenderXY_Plot(Figure* fig, dvis::XY_Plot* xy_plot)
{
	if(ui.m_plot->graphCount() < 1)
		ui.m_plot->addGraph();

	ui.m_plot->setWindowTitle(QString::fromStdString(fig->Title()));

	if(!m_title)
	{
		m_title = new QCPTextElement(ui.m_plot, QString::fromStdString(fig->Title()), QFont("sans", 12, QFont::Bold));
		// add title layout element:
		ui.m_plot->plotLayout()->insertRow(0);
		ui.m_plot->plotLayout()->addElement(0, 0, m_title);
	}
	ui.m_plot->plotLayout()->addElement(0, 0, m_title);

	ui.m_plot->graph(0)->setData
	(
		QVector<qreal>::fromStdVector(xy_plot->m_x),
		QVector<qreal>::fromStdVector(xy_plot->m_y)
	);
	ui.m_plot->xAxis->setLabel(QString::fromStdString(xy_plot->m_x_axis_bottom->m_label));
	ui.m_plot->yAxis->setLabel(QString::fromStdString(xy_plot->m_y_axis_left->m_label));

	// Set ranges if specified, else auto scale:
	if(false)
	{
		// Fixed axis range
		ui.m_plot->xAxis->setRange(xy_plot->m_x_axis_bottom->m_min, xy_plot->m_x_axis_bottom->m_max);
		ui.m_plot->yAxis->setRange(xy_plot->m_y_axis_left->m_min, xy_plot->m_y_axis_left->m_max);
	}
	else
	{
		ui.m_plot->rescaleAxes();
	}

	ui.m_plot->replot();
}
