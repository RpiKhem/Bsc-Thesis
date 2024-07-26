#ifndef FORMATWINDOW_H
#define FORMATWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

class formatWindow : public QDialog
{
    Q_OBJECT
public:
    explicit formatWindow(QWidget *parent = nullptr, QStringList options = {});
    QString getFormat() const {return comboBox->currentText();}

private:
    QPushButton *accept, *reject;
    QLabel *label;
    QComboBox *comboBox;
    QGridLayout *windowLayout;

};

#endif // FORMATWINDOW_H
