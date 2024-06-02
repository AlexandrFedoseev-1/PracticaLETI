#ifndef CELLWIDGET_H
#define CELLWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include "datareader.h"

class CellWidget : public QWidget {
    Q_OBJECT

public:
    explicit CellWidget(QWidget *parent = nullptr);
    void setCells(const std::vector<Cell>& newCells);
    void setCell(const Cell &cell);

public slots:
    void zoomIn();
    void zoomOut();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    void drawGrid(QPainter *painter);
    void drawCell(QPainter *painter, const Cell& cell);

    std::vector<Cell> cells;
    Cell currentCell;
    qreal scale; // переменная для масштаба

    QPoint lastMousePosition; // последняя позиция мыши
    bool dragging; // флаг для отслеживания состояния перетаскивания
    QPoint translation; // текущее смещение
};

#endif // CELLWIDGET_H
