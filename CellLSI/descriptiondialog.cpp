#include "descriptiondialog.h"
#include "datareader.h"
DescriptionDialog::DescriptionDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Описание");
    setModal(true);
    resize(400, 300);

    descriptionTextEdit = new QTextEdit(this);
    descriptionTextEdit->setReadOnly(true);

    saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &DescriptionDialog::saveDescription);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(descriptionTextEdit);
    layout->addWidget(saveButton);
    setLayout(layout);
}

void DescriptionDialog::setDescription(const Cell cell) {
    QPoint botomLeft(cell.bottomLeft.x, cell.bottomLeft.y);
    QPoint topRight(cell.topRight.x, cell.topRight.y);
    QRect rectCell(botomLeft,topRight);
    QPoint cellNamePoint = rectCell.center();

    currentDescription=QString::fromStdString(cell.name) + "\n";
    currentDescription+="B1(" + QString::number(cell.bottomLeft.x,'f',2)
                          + ", " + QString::number(cell.bottomLeft.y,'f',2)
                          + ") X(" + QString::number(cell.topRight.x,'f',2) + ");\n"
                          + "B1(" + QString::number(cell.bottomLeft.x,'f',2)
                          + ", " + QString::number(cell.bottomLeft.y,'f',2)
                          + ") Y(" + QString::number(cell.topRight.y,'f',2) + ");\n"
                          + "B1(" + QString::number(cell.bottomLeft.x,'f',2)
                          + ", " + QString::number(cell.topRight.y,'f',2)
                          + ") X(" + QString::number(cell.topRight.x,'f',2) + ");\n"
                          + "B1(" + QString::number(cell.topRight.x,'f',2)
                          + ", " + QString::number(cell.topRight.y,'f',2)
                          + ") Y(" + QString::number(cell.bottomLeft.y,'f',2) + ");\n";

    for (const auto& elemLine : cell.elementsLines) {

        if(elemLine.start.y == elemLine.end.y && elemLine.start.x == elemLine.end.x){
            currentDescription += QString::fromStdString(elemLine.fullname) + " (" +
                                  QString::number(elemLine.start.x,'f',2) + ", " +
                                  QString::number(elemLine.start.y,'f',2)+ ");\n";
        }
        else if(elemLine.start.x == elemLine.end.x){
            currentDescription += QString::fromStdString(elemLine.fullname) + " (" +
                                  QString::number(elemLine.start.x,'f',2) + ", " +
                                  QString::number(elemLine.start.y,'f',2) + ") Y(" +
                                  QString::number(elemLine.end.y,'f',2) + ");\n";
        }
        else if(elemLine.start.y == elemLine.end.y){
            currentDescription += QString::fromStdString(elemLine.fullname) + " (" +
                                  QString::number(elemLine.start.x,'f',2) + ", " +
                                  QString::number(elemLine.start.y,'f',2) + ") X(" +
                                  QString::number(elemLine.end.x,'f',2) + ");\n";
        }
    }
    if(!cell.contactPoints.empty()){
        for (const auto& contactPoint : cell.contactPoints) {
            currentDescription += "OR(NORTH) " + QString::fromStdString(contactPoint.name) + " (" +
                                  QString::number(contactPoint.coord.x,'f',2) + ", " +
                                  QString::number(contactPoint.coord.y,'f',2)+ ");\n";
        }
    }
    currentDescription+="H_TEXT(1.00) D_TEXT(-0.83) OR_TEXT(NORTH) TB(" +
                          QString::number(cellNamePoint.x(),'f',2) + ", " +
                          QString::number(cellNamePoint.y(),'f',2) + ", \"" +
                          QString::fromStdString(cell.name) + "\");";

    descriptionTextEdit->setText(currentDescription);
}

void DescriptionDialog::saveDescription() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить Описание", "", "Text Files (*.cpp);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << currentDescription;
            file.close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}
