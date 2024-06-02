#include "specificationdialog.h"

SpecificationDialog::SpecificationDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Specification");
    setModal(true);
    resize(400, 300);

    specificationTextEdit = new QTextEdit(this);
    specificationTextEdit->setReadOnly(true);

    saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &SpecificationDialog::saveSpecification);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(specificationTextEdit);
    layout->addWidget(saveButton);
    setLayout(layout);
}

void SpecificationDialog::setSpecification(const QString &cellName, const QString &specification) {
    currentSpecification = QString("%1\n\nSpecification:\n%2").arg(cellName).arg(specification);
    specificationTextEdit->setText(currentSpecification);
}

void SpecificationDialog::saveSpecification() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить Спецификацию", "", "Text Files (*.dat);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << currentSpecification;
            file.close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}

