/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtFigure.h"

class QtFigure : public QMainWindow
{
    Q_OBJECT

public:
    QtFigure(QWidget* parent = Q_NULLPTR);
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
