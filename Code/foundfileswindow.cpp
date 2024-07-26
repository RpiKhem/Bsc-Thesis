#include "foundfileswindow.h"

foundFilesWindow::foundFilesWindow(QWidget *parent, const FilePathModel *files) : QDialog(parent)
{
    accept = new QPushButton(tr("OK"), this);
    reject = new QPushButton(tr("Cancel"), this);

    label = new QLabel(tr("Found files:"), this);

    filesView = new QListView(this);
    filesView->setModel(const_cast<FilePathModel*>(files));
    //filesView->setModel(const_cast<QFileSystemModel*>(files));
    //filesView->setRootIndex(files->index(QDir::currentPath()));
    //filesView->setSelectionMode(QAbstractItemView::SingleSelection);

    windowLayout = new QGridLayout();

    windowLayout->addWidget(label, 0, 0);
    windowLayout->addWidget(filesView, 0, 1);
    windowLayout->addWidget(accept, 1, 0);
    windowLayout->addWidget(reject, 1, 1);

    setLayout(windowLayout);

    connect(accept, &QPushButton::clicked, this, &QDialog::accept);
    connect(reject, &QPushButton::clicked, this, &QDialog::reject);
}
