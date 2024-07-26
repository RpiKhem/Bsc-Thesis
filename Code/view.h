#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QTreeView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QInputDialog>

#include "model.h"
#include "foundfileswindow.h"
#include "formatwindow.h"
#include "waitdialog.h"

class View : public QWidget
{
    Q_OBJECT

public:
    View(QWidget *parent = nullptr);
    ~View();

private:
    Model *model;

    QVBoxLayout *mainLayout;
    QHBoxLayout *treeLayout, *buttonLayout;

    QTreeView *dirView, *fileView;

    QPushButton *delButton, *renameButton, *searchButton, *ebookButton, *imgButton;

    foundFilesWindow *foundFiles;
    formatWindow *formats;

    waitDialog *waitAnim;

signals:
    void searchFile(QModelIndex index, QString fileName);
    void fileChoosen(QString path);

    void updateFileMod(QModelIndex index);
    void openFile(QModelIndex index);

    void reqDelete(QModelIndex index);

    void reqRename(QModelIndex index, QString fileName);

    void reqCheckEbook(QModelIndex index);
    void reqCheckImg(QModelIndex index);

    void imgFormatChoosen(QModelIndex index, QString format);
    void ebookFormatChoosen(QModelIndex index, QString format);

private slots:
    void dirViewClicked(const QModelIndex &index);
    void fileViewDoubleClicked(const QModelIndex &index);
    void setNewRoot(QModelIndex index);

    void delButtonClicked();
    void renameButtonClicked();
    void searchButtonClicked();
    void ebookButtonClicked();
    void imgButtonClicked();

    void displayError(const QString& errorMsg);
    void displayInfo(const QString& jobMsg);

    void showFoundFiles(const FilePathModel* foundFilesModel);

    void showOptionsImg(const QStringList& options);
    void showOptionsEbook(const QStringList& options);

    void oneFileFound(QModelIndex index);

    void waitAnimationStart();
    void waitAnimationStop();
};
#endif // VIEW_H
