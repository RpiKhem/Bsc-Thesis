#include "model.h"

Model::Model(QObject *parent)
    : QObject{parent}
{
    sPath = "";

    setDirModel(sPath);
    setFileModel(sPath);

    imgFormats = {"jpg", "png", "bmp"};
    ebookFormats = {"epub", "mobi", "azw3", "pdf"};
}

QFileSystemModel *Model::getDirModel() const
{
    return dirModel;
}

QFileSystemModel *Model::getFileModel() const
{
    return fileModel;
}

void Model::updateFileModel(QModelIndex index)
{
    sPath = dirModel->fileInfo(index).absoluteFilePath();
    QModelIndex indexBack = fileModel->setRootPath(sPath);
    emit modelUpdated(indexBack);
}

void Model::fileOpen(const QModelIndex index)
{
    QString oPath = fileModel->fileInfo(index).absoluteFilePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(oPath));
}

void Model::deleteFunction(QModelIndex index)
{
    if (!index.isValid()){
        emit errorOccurred("No file choosen.");
        return;
    }

    bool ok;
    if (fileModel->fileInfo(index).isDir()) {
        // can not remove folder which should be not present int fileModel=listView
        emit errorOccurred("Failed to delete directory from file tab.");;
        return;
    }
    else{
        ok = fileModel->remove(index);
    }

    if (!ok){
        emit errorOccurred("Failed to delete choosen file or directory.");
    }
    else{
        emit jobDone("File deleted successfully.");
    }
}

void Model::renameFunction(QModelIndex index, QString newName)
{
    // check valid index
    indexCheck(index);

    if (newName.isEmpty()) {
        emit errorOccurred("New name can not be empty.");
        return;
    }

    QString sPath = fileModel->fileInfo(index).absoluteFilePath();

    QFile file(sPath);
    if(file.exists()){
        // get only the path for the file, to create absolute path with this and new name
        QString fPath = sPath.section("/",0,-2);
        // full file name
        QString sFile = sPath.section("/",-1,-1);
        //file extension only
        QString sFileFormat = sFile.section(".",-1,-1);
        // important: the section method cuts the last "/", which then leads the file to one folder up after rename
        QString newFileName = fPath + "/" + newName + "." + sFileFormat;

        if(!file.rename(newFileName)){
            emit errorOccurred("Failed to rename file.");
        }

    }
    else{
        emit errorOccurred("File does not exists.");
    }
}

void Model::searchFileFunction(QModelIndex index, QString fileName)
{
    if(fileName.isEmpty()){
        emit errorOccurred("Empty file name.");
        return;
    }

    //multiple check to determine root -> if file is higlighted then files folder, if not then the folder, if not then the root
    QString sDir = sPath;
    if (!index.isValid()){
        //if file was highlighted, get the files directory
        if (!fileModel->isDir(index)){
            sDir = fileModel->fileInfo(index).absoluteFilePath().section("/",0,-2);
        }
        //if directory was higlighted, set that dir to root for te search
        else{
            sDir = dirModel->fileInfo(index).absolutePath();
        }
    }

    QDirIterator it(sDir, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QList<QString> found;
    QRegularExpression re(fileName, QRegularExpression::CaseInsensitiveOption);
    while (it.hasNext()) {
        it.next();
        if (re.match(it.fileName()).hasMatch()) {
            found.append(it.filePath());
        }
    }

    if(found.isEmpty()){
        emit errorOccurred("Could not find the requested file.\nPlease check file name, or start search for an outer directory.");
    }
    else if(found.size()==1){

        emit foundFile(fileModel->setRootPath(found.first().section("/",0,-2)));
        emit jobDone("One file has been found.");
    }
    else{
        //create a list or qmodel to make a list on view layer
        FilePathModel* foundFilesModel = new FilePathModel(found);

        emit foundFiles(foundFilesModel);
    }
}

void Model::filePathToSet(QString path)
{
    emit foundFile(fileModel->setRootPath(path.section("/",0,-2)));
}

void Model::checkImage(QModelIndex index)
{
    indexCheck(index);
    emit availableOptionsImg(this->imgFormats);
}

void Model::imgConvFunction(QModelIndex index, QString format)
{
    QString sPath = fileModel->fileInfo(index).absoluteFilePath();
    // full file name
    QString sFile = sPath.section("/",-1,-1);
    //file extension only
    QString sFileFormat = sFile.section(".",-1,-1);
    //filename without extension
    QString sFileName = sFile.section(".",-0,-2);
    //full file path without file name
    QString sDir = sPath.section("/",0,-2);

    //QStringList formats = {"jpg", "png", "bmp"};

    //input image format check
    bool sFound = (std::find(this->imgFormats.begin(), this->imgFormats.end(), sFileFormat) != this->imgFormats.end());
    if(!sFound){
        emit errorOccurred("Not (yet) supported source format.");
        return;
    }

    //desired image fromat check
    bool found = (std::find(this->imgFormats.begin(), this->imgFormats.end(), format) != this->imgFormats.end());
    if(!found){
        emit errorOccurred("Can not find format or format is not yet supported.");
        return;
    }
    if(format == sFileFormat){
        emit errorOccurred("Can not convert into the source format.");
        return;
    }

    //convert
    QImage image = QImage(sPath);
    if(!image.isNull()){
        QString newName = sDir + "/" + sFileName + "." + format;

        if(image.save(newName, 0, -1)){
            emit jobDone("Image converted successfully.");
        }
        else{
            emit errorOccurred("Convert failed. Please try again.");
        }
    }
}

void Model::checkEbook(QModelIndex index)
{
    indexCheck(index);
    emit availableOptionsEbook(this->ebookFormats);
}

void Model::ebookConvFunction(QModelIndex index, QString format)
{
    QString sPath = fileModel->fileInfo(index).absoluteFilePath();
    // full file name
    QString sFile = sPath.section("/",-1,-1);
    //file extension only
    QString sFileFormat = sFile.section(".",-1,-1);
    //filename without extension
    QString sFileName = sFile.section(".",-0,-2);
    QString sDir = sPath.section("/",0,-2);

    //source ebook format check
    bool sFound = (std::find(this->ebookFormats.begin(), this->ebookFormats.end(), sFileFormat) != this->ebookFormats.end());
    if(!sFound){
        emit errorOccurred("Not (yet) supported source format.");
        return;
    }

    //desired ebook format check

    bool found = (std::find(this->ebookFormats.begin(), this->ebookFormats.end(), format) != this->ebookFormats.end());
    if(!found){
        emit errorOccurred("Can not find format or format is not yet supported.");
        return;
    }
    if(format == sFileFormat){
       emit errorOccurred("Can not convert into the source format.");
        return;
    }


    //convert
    QString rFileName = sFileName + "." + format;

    convThread = new ebookConvThread(this);
    convThread->setObjectName("converter");
    QObject::connect(convThread, &ebookConvThread::processFinished, this, &Model::ebookConvFinished);

    //Start convert thread with arguments
    convThread->setCommand(sDir, "ebook-convert", QStringList() << sFile << rFileName);
    convThread->start();

    emit startWait();
}

void Model::setDirModel(QString sPath)
{
    dirModel = new QFileSystemModel(this);
    dirModel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    dirModel->setRootPath(sPath);
}

void Model::setFileModel(QString sPath)
{
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    fileModel->setRootPath(sPath);
}

void Model::indexCheck(QModelIndex index)
{
    if (!index.isValid()){
        emit errorOccurred("No file choosen.");
        return;
    }
}

void Model::ebookConvFinished()
{
    emit stopWait();
    delete convThread;
    emit jobDone("E-book converted successfully.");
}
