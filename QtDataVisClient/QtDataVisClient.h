/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once
#include <vector>

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
    Ui::QtDataVisClientClass ui;

    std::vector<QPointer<QtFigure>> m_figures;
};
