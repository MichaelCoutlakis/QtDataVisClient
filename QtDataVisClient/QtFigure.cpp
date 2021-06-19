/***************************************************************************//**
* \brief		
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#include "QtFigure.h"
#include <QMessageBox>

#include "qcustomplot.h"

QtFigure::QtFigure(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

   // connect(ui.pushButton, &QPushButton::pressed, this, &QtDataVisClient::OnConnect);

    // generate some data:
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i = 0; i < 101; ++i)
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


//void QtDataVisClient::OnConnect()
//{
//    QMessageBox::information(this, tr("asdf"), tr("OnConnect"), QMessageBox::Ok);
//}