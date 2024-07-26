#include "ebookconvthread.h"

ebookConvThread::ebookConvThread(QObject *parent)
    : QThread{parent}
{

}

void ebookConvThread::setCommand(const QString& workDir, const QString &program, const QStringList &arguments)
{
    dir = workDir;
    prog = program;
    args = arguments;
}

void ebookConvThread::run()
{
    QProcess process;
    process.setWorkingDirectory(dir);
    process.start(prog, args);
    process.waitForFinished(-1);
    emit processFinished();
}
