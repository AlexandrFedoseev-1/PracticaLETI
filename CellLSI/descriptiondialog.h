#ifndef DESCRIPTIONDIALOG_H
#define DESCRIPTIONDIALOG_H
#include "datareader.h"
#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

class DescriptionDialog : public QDialog {
    Q_OBJECT

public:
    explicit DescriptionDialog(QWidget *parent = nullptr);
    void setDescription(const Cell cell);

private slots:
    void saveDescription();

private:
    QTextEdit *descriptionTextEdit;
    QPushButton *saveButton;
    QString currentDescription;
};

#endif // DESCRIPTIONDIALOG_H
