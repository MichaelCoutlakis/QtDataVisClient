/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once

#include "../../CppDataVisLib/CppDataVisLib/Figure.h"
#include "../../CppDataVisLib/CppDataVisLib/XY_Plot.h"


#include <QtWidgets/QMainWindow>
#include "ui_QtFigure.h"

class QtFigure : public QMainWindow, public dvis::Backend, public dvis::Figure
{
    Q_OBJECT

public:
    QtFigure(dvis::Figure figure, QWidget* parent = Q_NULLPTR);
    virtual ~QtFigure() {}

    void SetFigure(const dvis::Figure& fig);


    void RenderFigure(Figure* fig) override {};
    void RenderXY_Plot(Figure* fig, dvis::XY_Plot* pXY_Plot) override {};


private slots:

signals:
    void FigureClosed(QtFigure* figure);
private:
    Ui::QtFigureClass ui;

    void closeEvent(QCloseEvent* event) override
    {
        FigureClosed(this);
        QMainWindow::closeEvent(event);
    }
};
#pragma once
