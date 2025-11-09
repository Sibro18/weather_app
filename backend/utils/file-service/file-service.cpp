#include "file-service.h"
#include <QFile>
#include <QSaveFile>
#include <QDir>

namespace Common
{
    FileService::FileService(QObject* parent)
        : QObject(parent)
    {}

    FileService::~FileService()
    {
        QMutexLocker locker(&_mapAccessMutex);
        _fileMutexMap.clear();
    }

    void FileService::saveData(const QMap<QString, QByteArray> &fileNameDataMap)
    {
        for (const auto &[path, data] : fileNameDataMap.asKeyValueRange())
        {
            QMutex* fileMutex = _getMutexForFile(path);
            QMutexLocker locker(fileMutex);


            QSaveFile file(path);

            if (!file.open(QIODevice::WriteOnly))
            {
                continue;
            }

            if (file.write(data) != data.size())
            {
                file.cancelWriting();

                continue;
            }
        }
    }

    std::optional<QByteArray> FileService::getData(const QString &filePath)
    {
        QMutex* fileMutex = _getMutexForFile(filePath);
        QMutexLocker locker(fileMutex);

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly))
        {
            return {};
        }

        return { file.readAll() };
    }

    std::optional<QStringList> FileService::getFileNamesByDir(const QString& dirPath) const
    {
        QDir dir(dirPath);

        if (!dir.exists())
        {
            return {};
        }

        return { dir.entryList() };
    }

    bool FileService::createNewDirIfNotExists(const QString &path) const
    {
        if (path.trimmed().isEmpty()) {
            return false;
        }

        QDir dir(path);

        if (dir.exists()) {
            return true;
        }

        if (!dir.mkpath(".")) {
            return false;
        }

        return true;
    }

    QMutex* FileService::_getMutexForFile(const QString& fileName)
    {
        QMutexLocker locker(&_mapAccessMutex);

        const QString key = QDir::cleanPath(fileName);
        QSharedPointer<QMutex> &mutexPtr = _fileMutexMap[key];

        if (!mutexPtr)
        {
            mutexPtr = QSharedPointer<QMutex>::create();
        }

        return mutexPtr.get();
    }
}
