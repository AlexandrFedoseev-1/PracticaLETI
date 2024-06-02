#include "macroblockdialog.h"

MacroblockDialog::MacroblockDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Спецификация макроблока");
    setModal(true);
    resize(400, 300);

    macroblockTextEdit = new QTextEdit(this);
    macroblockTextEdit->setReadOnly(true);

    saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &MacroblockDialog::saveMacroblock);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(macroblockTextEdit);
    layout->addWidget(saveButton);
    setLayout(layout);
}

void MacroblockDialog::setMacroblock(const std::vector<NamePoint> strings) {
    currentMacroblock = "";
    for (const auto& str : strings) {
        currentMacroblock+=QString::fromStdString(str.name) + "\n";
    }

    macroblockTextEdit->setText(currentMacroblock);
}

void MacroblockDialog::saveMacroblock() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить Макроблок", "", "Text Files (*.dat);;All Files (*)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << currentMacroblock;
            file.close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}
