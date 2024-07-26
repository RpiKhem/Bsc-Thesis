#include "waitdialog.h"

waitDialog::waitDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Please wait...");
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setModal(true); // Make the dialog modal to block input to other windows (until it is clicked)

    QHBoxLayout* layout = new QHBoxLayout(this);
    mazeAnim = new Maze(this);
    layout->addWidget(mazeAnim);
}
