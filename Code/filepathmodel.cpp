#include "filepathmodel.h"

FilePathModel::FilePathModel(const QStringList& filePaths, QObject* parent)
    : QAbstractListModel(parent)
    , m_filePaths(filePaths)
{
}

int FilePathModel::rowCount(const QModelIndex& /*parent*/) const
{
    return m_filePaths.count();
}

QVariant FilePathModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_filePaths.count())
        return QVariant();

    if (role == Qt::DisplayRole)
        return m_filePaths[index.row()];

    return QVariant();
}

QString FilePathModel::filePathAt(const QModelIndex& index) const
{
    if (!index.isValid() || index.row() >= m_filePaths.count())
        return QString();

    return m_filePaths[index.row()];
}
