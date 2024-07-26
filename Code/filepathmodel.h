#ifndef FILEPATHMODEL_H
#define FILEPATHMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class FilePathModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FilePathModel(const QStringList& filePaths, QObject* parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    // Custom method to retrieve the file path at a given index
    QString filePathAt(const QModelIndex& index) const;

private:
    QStringList m_filePaths;
};
#endif // FILEPATHMODEL_H
