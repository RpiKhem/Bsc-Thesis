#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>
#include <QObject>
#include <QHBoxLayout>
#include "maze.h"

class waitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit waitDialog(QWidget* parent = nullptr);

private:
    Maze *mazeAnim;
};

#endif // WAITDIALOG_H
