#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include "cellwidget.h"
#include "specificationdialog.h"
#include "datareader.h"
#include "descriptiondialog.h"
#include "macroblockdialog.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // Основное окно
    QMainWindow window;
    QWidget centralWidget;
    QVBoxLayout layout(&centralWidget);
    window.setGeometry(0,0,600,600);

    // Виджет для выбора ячейки
    QComboBox cellSelector;
    layout.addWidget(&cellSelector);

    QHBoxLayout toolsLayout;
    QPushButton openButton("Открыть файл");
    toolsLayout.addWidget(&openButton);

    // Кнопка для открытия окна "Описание"
    QPushButton descButton("Описание");
    toolsLayout.addWidget(&descButton);

    // Кнопка для открытия окна "Макроблок"
    QPushButton macrButton("Макроблок");
    toolsLayout.addWidget(&macrButton);

    // Кнопка для открытия окна спецификации
    QPushButton specButton("Спецификация");
    toolsLayout.addWidget(&specButton);
    layout.addLayout(&toolsLayout);

    // Виджет для отображения ячейки
    CellWidget cellWidget;
    layout.addWidget(&cellWidget);

    // Создание окна спецификации
    SpecificationDialog specDialog;
    specDialog.setWindowModality(Qt::NonModal);

    // Создание окна описания
    DescriptionDialog descDialog;
    descDialog.setWindowModality(Qt::NonModal);

    // Создание окна макроблока
    MacroblockDialog macrDialog;
    macrDialog.setWindowModality(Qt::NonModal);

    // Объявляем данные
    Data data;
    std::vector<Cell> cells;

    // Сигнал для кнопки "Open"
    QObject::connect(&openButton, &QPushButton::clicked, [&]() {

        QString fileName = QFileDialog::getOpenFileName(&window, "Open File", "", "Text Files (*.txt);;All Files (*)");
        if (!fileName.isEmpty()) {
            // Загрузка данных
            data = readLinesFromFile(fileName.toStdString());
            if(!data.borderLines.empty()){
                data.cells= findCellCoordinates(findIntersectionPoints(data.borderLines), data);
                cells = moveAllCellsToOrigin(data.cells);

                // Обновление выпадающего списка
                cellSelector.clear();
                for (const auto& cell : cells) {
                    cellSelector.addItem(QString::fromStdString(cell.name));
                }
                macrDialog.setMacroblock(data.strings);
            }else  QMessageBox::critical(&window,"Ошибка", "Путь к фалу не должен содержать русских символов: " + fileName);

        }
    });

    // Сигнал для обновления отображения при выборе ячейки
    QObject::connect(&cellSelector, &QComboBox::currentTextChanged, [&](const QString &name) {
        auto it = std::find_if(cells.begin(), cells.end(), [&](const Cell& cell) {
            return cell.name == name.toStdString();
        });
        if (it != cells.end()) {
            cellWidget.setCell(*it);
            cellWidget.paintingActive();
            specDialog.setSpecification(QString::fromStdString(it->name), QString::fromStdString(it->specification));
            descDialog.setDescription(*it);
        }
    });
    // Сигнал для открытия окна описания
    QObject::connect(&descButton, &QPushButton::clicked, [&]() {
        descDialog.show();
        descDialog.raise();
    });

    // Сигнал для открытия окна спецификации
    QObject::connect(&specButton, &QPushButton::clicked, [&]() {
        specDialog.show();
        specDialog.raise();
    });

    // Сигнал для открытия окна макроблока
    QObject::connect(&macrButton, &QPushButton::clicked, [&]() {
        macrDialog.show();
        macrDialog.raise();
    });

    // Отображение основного окна
    window.setCentralWidget(&centralWidget);
    window.show();

    return a.exec();
}
