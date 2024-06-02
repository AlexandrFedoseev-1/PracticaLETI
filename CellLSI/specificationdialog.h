#ifndef SPECIFICATIONDIALOG_H
#define SPECIFICATIONDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDialog>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

class SpecificationDialog : public QDialog {
    Q_OBJECT

public:
    explicit SpecificationDialog(QWidget *parent = nullptr);
    void setSpecification(const QString &cellName, const QString &specification);

private slots:
    void saveSpecification();

private:
    QTextEdit *specificationTextEdit;
    QPushButton *saveButton;
    QString currentSpecification;
};

#endif // SPECIFICATIONDIALOG_H
