/***************************************************************************//**
* \brief
* \copyright	Michael Coutlakis 2021
* \license		GNU General Public License v3.0, see the LICENSE file.
*******************************************************************************/
#pragma once

#include "CppDataVisLib/Figure.h"
#include "CppDataVisLib/XY_Plot.h"


#include <QtWidgets/QMainWindow>
#include "ui_QtFigure.h"

#include "qcustomplot.h"

class QtPlot : public QObject
{
    Q_OBJECT
public:
    virtual void Draw(QCustomPlot* uiplot) = 0;
};

class QtXY_Plot : public QtPlot, public dvis::XY_Plot
{
    Q_OBJECT

public:
    QtXY_Plot(
        const std::vector<double>& x,
        const std::vector<double>& y,
        const std::string& x_label = "",
        const std::string& y_label = ""
    ) :
        dvis::XY_Plot(x, y, x_label, y_label)
    { }
    // FIXME naughty
    //void SetXY_Plot(const dvis::XY_Plot& plot) { static_cast<dvis::XY_Plot>(*this) = plot; }


    void Draw(QCustomPlot* uiplot) override;
};

class QtFigure : public QMainWindow,  public dvis::Figure
{
    Q_OBJECT

public:
    QtFigure(dvis::Figure figure, QWidget* parent = Q_NULLPTR);
    virtual ~QtFigure() {}

    void SetFigure(const dvis::Figure& fig);

    // TODO: Pattern?
    void Draw();
    //void RenderFigure(Figure* fig) override;
    // This was sorely misplaced... Render XY plot in figure...
    //void RenderXY_Plot(Figure* fig, dvis::XY_Plot* xy_plot) override;


    std::shared_ptr<QtPlot> m_plot;
private slots:

signals:
    void FigureClosed(QtFigure* figure);
private:
    Ui::QtFigureClass ui;

    QCPTextElement* m_title = nullptr;

    void closeEvent(QCloseEvent* event) override
    {
        FigureClosed(this);
        QMainWindow::closeEvent(event);
    }
};
#pragma once
