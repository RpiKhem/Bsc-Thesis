#ifndef EBOOKCONVTHREAD_H
#define EBOOKCONVTHREAD_H

#include <QThread>
#include <QObject>
#include <QCoreApplication>
#include <QProcess>

class ebookConvThread : public QThread
{
    Q_OBJECT
public:
    explicit ebookConvThread(QObject *parent = nullptr);
    void setCommand(const QString& workDir, const QString& program, const QStringList& arguments);
protected:
    void run() override;
private:
    QString prog, dir;
    QStringList args;
signals:
    void processFinished();
};

#endif // EBOOKCONVTHREAD_H
