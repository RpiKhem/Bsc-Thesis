#include "view.h"

View::View(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("FileManager");
    setMinimumSize(1280, 720);

    model = new Model(this);

    delButton = new QPushButton("Delete", this);
    renameButton = new QPushButton("Rename", this);
    searchButton = new QPushButton("Search", this);
    ebookButton = new QPushButton("Convert E-book", this);
    imgButton = new QPushButton("Convert Image", this);

    dirView = new QTreeView(this);
    dirView->setModel(model->getDirModel());
    dirView->setColumnWidth(0,250);

    fileView = new QTreeView(this);
    fileView->setModel(model->getFileModel());
    fileView->setColumnWidth(0, 250);

    mainLayout = new QVBoxLayout();
    treeLayout = new QHBoxLayout();
    buttonLayout = new QHBoxLayout();



    // always check the sequence
    treeLayout->addWidget(dirView);
    treeLayout->addWidget(fileView);

    buttonLayout->addWidget(delButton);
    buttonLayout->addWidget(renameButton);
    buttonLayout->addWidget(searchButton);
    buttonLayout->addWidget(ebookButton);
    buttonLayout->addWidget(imgButton);

    mainLayout->addLayout(treeLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    //connects
    connect(dirView, &QTreeView::clicked, this, &View::dirViewClicked);
    connect(fileView, &QTreeView::doubleClicked, this, &View::fileViewDoubleClicked);

    connect(delButton, &QPushButton::clicked, this, &View::delButtonClicked);
    connect(renameButton, &QPushButton::clicked, this, &View::renameButtonClicked);
    connect(searchButton, &QPushButton::clicked, this, &View::searchButtonClicked);
    connect(ebookButton, &QPushButton::clicked, this, &View::ebookButtonClicked);
    connect(imgButton, &QPushButton::clicked, this, &View::imgButtonClicked);

    connect(this, &View::updateFileMod, model, &Model::updateFileModel);
    connect(model, &Model::modelUpdated, this, &View::setNewRoot);

    connect(this, &View::openFile, model, &Model::fileOpen);

    connect(this, &View::reqDelete, model, &Model::deleteFunction);

    connect(this, &View::reqRename, model, &Model::renameFunction);

    connect(this, &View::reqCheckEbook, model, &Model::checkEbook);
    connect(this, &View::reqCheckImg, model, &Model::checkImage);

    connect(model, &Model::errorOccurred, this, &View::displayError);
    connect(model, &Model::jobDone, this, &View::displayInfo);

    connect(this, &View::searchFile, model, &Model::searchFileFunction);
    connect(model, &Model::foundFile, this, &View::oneFileFound);
    connect(model, &Model::foundFiles, this, &View::showFoundFiles);
    connect(this, &View::fileChoosen, model, &Model::filePathToSet);

    connect(model, &Model::availableOptionsImg, this, &View::showOptionsImg);
    connect(this, &View::imgFormatChoosen, model, &Model::imgConvFunction);

    connect(model, &Model::availableOptionsEbook, this, &View::showOptionsEbook);
    connect(this, &View::ebookFormatChoosen, model, &Model::ebookConvFunction);

    connect(model, &Model::startWait, this, &View::waitAnimationStart);
    connect(model, &Model::stopWait, this, &View::waitAnimationStop);

}

View::~View()
{
}

void View::dirViewClicked(const QModelIndex &index)
{
    emit updateFileMod(index);
}

void View::fileViewDoubleClicked(const QModelIndex &index)
{
    emit openFile(index);
}

void View::setNewRoot(QModelIndex index)
{
    fileView->setRootIndex(index);
}

void View::delButtonClicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete file", "Are you sure you want to delete the file?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes){
        emit reqDelete(fileView->currentIndex());
    }
}

void View::renameButtonClicked()
{    
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setWindowTitle("Rename");
    dialog.setLabelText("New file name:");
    int result = dialog.exec();

    if (result == QInputDialog::Accepted) {
        QString newName = dialog.textValue();
        emit reqRename(fileView->currentIndex(), newName);
    }
    else{
        return;
    }

}

void View::searchButtonClicked()
{
    QInputDialog dialog;
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setWindowTitle("Search for file");
    dialog.setLabelText("File name:");
    int result = dialog.exec();

    if (result == QInputDialog::Accepted) {
        QString fileName = dialog.textValue();
        QModelIndex fileSelect = fileView->currentIndex();
        if(fileSelect.isValid()){
            emit searchFile(fileSelect, fileName);
        }
        else{
            emit searchFile(dirView->currentIndex(), fileName);
        }
    }
    else{
        return;
    }

}

void View::ebookButtonClicked()
{
    emit reqCheckEbook(fileView->currentIndex());
}

void View::imgButtonClicked()
{
    emit reqCheckImg(fileView->currentIndex());
}

void View::displayError(const QString &errorMsg)
{
    QMessageBox::critical(this, "Error", errorMsg);
}

void View::displayInfo(const QString &jobMsg)
{
    QMessageBox::information(this, "Information", jobMsg);
}

void View::showFoundFiles(const FilePathModel* foundFilesModel)
{
    foundFiles = new foundFilesWindow(this, foundFilesModel);
    if (foundFiles->exec() == foundFilesWindow::Accepted){
        emit fileChoosen(foundFiles->getChoosenFile());
    }
}

void View::showOptionsImg(const QStringList &options)
{
    formats = new formatWindow(this, options);
    if (formats->exec() == formatWindow::Accepted){
        emit imgFormatChoosen(fileView->currentIndex(), formats->getFormat());
    }
}

void View::showOptionsEbook(const QStringList &options)
{
    formats = new formatWindow(this, options);
    if (formats->exec() == formatWindow::Accepted){
        emit ebookFormatChoosen(fileView->currentIndex(), formats->getFormat());
    }
}

void View::oneFileFound(QModelIndex index)
{
    fileView->setRootIndex(index);
}

void View::waitAnimationStart()
{
    waitAnim = new waitDialog(this);
    waitAnim->show();
}

void View::waitAnimationStop()
{
    waitAnim->close();
    delete waitAnim;
}

