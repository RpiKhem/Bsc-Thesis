#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QtGui>
#include "filepathmodel.h"

//for the format checks
#include <algorithm>

#include "ebookconvthread.h"


class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

    QFileSystemModel* getDirModel() const;
    QFileSystemModel* getFileModel() const;

    void updateFileModel(QModelIndex index);
    void fileOpen(const QModelIndex index);

    void deleteFunction(QModelIndex index);
    void renameFunction(QModelIndex index, QString newName);

    void searchFileFunction(QModelIndex index, QString fileName);
    void filePathToSet(QString path);

    void checkImage(QModelIndex index);
    void imgConvFunction(QModelIndex index, QString format);

    void checkEbook(QModelIndex index);
    void ebookConvFunction(QModelIndex index, QString format);

signals:
    void errorOccurred(const QString& errorMsg);
    void jobDone(const QString& jobMsg);

    void modelUpdated(QModelIndex index);

    void foundFile(QModelIndex index);
    void foundFiles(const FilePathModel* foundFilesModel);

    void availableOptionsImg(const QStringList& options);
    void availableOptionsEbook(const QStringList& options);

    void startWait();
    void stopWait();

private slots:


private:
    QString sPath;

    QFileSystemModel *dirModel;
    QFileSystemModel *fileModel;

    QStringList imgFormats, ebookFormats;

    void setDirModel(QString sPath);
    void setFileModel(QString sPath);

    void indexCheck(QModelIndex index);

    ebookConvThread * convThread;
    void ebookConvFinished();
};

#endif // MODEL_H
