#include "cellwidget.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QColor>
#include <map>

int STEP = 20;

CellWidget::CellWidget(QWidget *parent) :QWidget(parent),scale(1.0) {

}


void CellWidget::setCell(const Cell &cell) {
    currentCell = cell;
    update();
}

void CellWidget::zoomIn() {
    scale *= 1.1; // Увеличение масштаба на 20%
    update();
}

void CellWidget::zoomOut() {
    scale /= 1.1; // Уменьшение масштаба на 20%
    update();
}

void CellWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(100 + translation.x(), 460 + translation.y());
    painter.scale(scale, scale);
        // Рисование координатной сетки
    drawGrid(&painter);
}
void CellWidget::wheelEvent(QWheelEvent *event) {
    // Получаем значение прокрутки колеса мыши
    int delta = event->angleDelta().y();

    // Изменяем масштаб в зависимости от направления прокрутки
    if (delta > 0) {
        zoomIn();
    } else {
        zoomOut();
    }
}


void CellWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePosition = event->pos();
        dragging = true;
    }
}

void CellWidget::mouseMoveEvent(QMouseEvent *event) {
    if (dragging) {
        QPoint delta = event->pos() - lastMousePosition;
        translation += delta;
        lastMousePosition = event->pos();
        update();
    }
}

void CellWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
    }
}

void CellWidget::drawGrid(QPainter *painter) {
    QPen pen(Qt::lightGray, 1, Qt::DotLine);
    painter->setPen(pen);
    int offsetY = -5000;
    int offsetX = -500;
    int step = 20;
    for (int x = offsetX; x < width()/scale; x += step) {
        painter->drawLine(x, offsetY, x, height()/scale);
    }
    for (int y = offsetY; y < height()/scale; y += step) {
        painter->drawLine(offsetX, y, width()/scale, y);
    }

    pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->drawLine(offsetX , 0 , width()/scale, 0);
    painter->drawLine(0, offsetY , 0, height()/scale);
    drawCell(painter, currentCell);
}

// Функция для получения цвета на основе имени
QColor getColorForName(const std::string& name) {
    static std::map<std::string, QColor> nameToColorMap;
    if (nameToColorMap.find(name) == nameToColorMap.end()) {
        // Если имя не найдено в карте, создаем новый случайный цвет для этого имени
        QColor color = QColor::fromRgb(rand() % 256, rand() % 256, rand() % 256);
        nameToColorMap[name] = color;
    }
    return nameToColorMap[name];
}

void CellWidget::drawCell(QPainter *painter, const Cell& cell) {
    QPen pen(Qt::black, 2);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    QRectF rect(cell.bottomLeft.x*STEP, cell.bottomLeft.y*-STEP,
                cell.topRight.x*STEP - cell.bottomLeft.x*STEP,
                cell.topRight.y*-STEP - cell.bottomLeft.y*-STEP);
    painter->drawRect(rect);
    painter->drawText(rect.center(),QString::fromStdString(cell.name));

    for (const auto& line : cell.elementsLines) {
        QColor color = getColorForName(line.name);
        pen.setColor(color);
        pen.setWidth(1);
        painter->setPen(pen);

        painter->drawLine(line.start.x*STEP, line.start.y*-STEP, line.end.x*STEP, line.end.y*-STEP);
        painter->drawEllipse(QPointF(line.start.x*STEP, line.start.y*-STEP), 2, 2);
        painter->drawEllipse(QPointF(line.end.x*STEP, line.end.y*-STEP), 2, 2);

        // Рисование имени линии
        painter->drawText(QPointF(line.start.x*STEP - line.name.size()*STEP/4 , line.start.y*-STEP-5), QString::fromStdString(line.name));

    }

    for (const auto& point : cell.contactPoints) {
        QColor color = getColorForName(point.name);
        pen.setColor(color);
        painter->setPen(pen);

        painter->drawEllipse(QPointF(point.coord.x*STEP, point.coord.y*-STEP), 3, 3);

        // Рисование имени контактной точки
        painter->drawText(QPointF(point.coord.x*STEP + 5, point.coord.y*-STEP + 5), QString::fromStdString(point.name));
    }
}
