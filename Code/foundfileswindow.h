#ifndef FOUNDFILESWINDOW_H
#define FOUNDFILESWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QFileSystemModel>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QListView>
#include "filepathmodel.h"

class foundFilesWindow : public QDialog
{
    Q_OBJECT
public:
    explicit foundFilesWindow(QWidget *parent = nullptr,const  FilePathModel* files = nullptr);
    QString getChoosenFile(){return filesView->selectionModel()->currentIndex().data().toString();};

signals:

private:
    QPushButton *accept, *reject;
    QLabel *label;
    QListView *filesView;
    QGridLayout *windowLayout;

};

#endif // FOUNDFILESWINDOW_H
