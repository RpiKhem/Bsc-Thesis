#include "formatwindow.h"

formatWindow::formatWindow(QWidget *parent, QStringList options) : QDialog(parent)
{
    accept = new QPushButton(tr("OK"), this);
    reject = new QPushButton(tr("Cancel"), this);

    label = new QLabel(tr("Format:"), this);

    comboBox = new QComboBox(this);
    comboBox->addItems(options);
    //comboBox->show();


    windowLayout = new QGridLayout();

    windowLayout->addWidget(label, 0, 0);
    windowLayout->addWidget(comboBox, 0, 1);
    windowLayout->addWidget(accept, 1, 0);
    windowLayout->addWidget(reject, 1, 1);

    setLayout(windowLayout);

    connect(accept, &QPushButton::clicked, this, &QDialog::accept);
    connect(reject, &QPushButton::clicked, this, &QDialog::reject);
}
