#ifndef MACROBLOCKDIALOG_H
#define MACROBLOCKDIALOG_H
#include "datareader.h"
#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
class MacroblockDialog : public QDialog {
    Q_OBJECT

public:
    explicit MacroblockDialog(QWidget *parent = nullptr);
    void setMacroblock(const std::vector<NamePoint> strings);

private slots:
    void saveMacroblock();

private:
    QTextEdit *macroblockTextEdit;
    QPushButton *saveButton;
    QString currentMacroblock;
};

#endif // MACROBLOCKDIALOG_H
