#include "file-service.h"
#include <QFile>
#include <QSaveFile>
#include <QDebug>
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

            QString baseLogInfo = QString("[FileService].saveData(path: %0)").arg(path);
            qWarning() << baseLogInfo;

            QSaveFile file(path);

            if (!file.open(QIODevice::WriteOnly))
            {
                qWarning() << baseLogInfo << "\taccess error: " << file.errorString();

                continue;
            }

            if (file.write(data) != data.size())
            {
                qWarning() << baseLogInfo << "\twriting error: " << file.errorString();

                file.cancelWriting();

                continue;
            }

            if (!file.commit())
            {
                qWarning() << baseLogInfo << "\tcommit error: " << file.errorString();
            }
        }
    }

    std::optional<QByteArray> FileService::getData(const QString &filePath)
    {
        QMutex* fileMutex = _getMutexForFile(filePath);
        QMutexLocker locker(fileMutex);

        QFile file(filePath);

        QString baseLogInfo = QString("[FileService].getData(path: %0)").arg(filePath);

        if (!file.open(QIODevice::ReadOnly))
        {
            qWarning() << baseLogInfo << "\taccess error: " << file.errorString();

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
            qWarning() << "[FileService] Empty path provided";
            return false;
        }

        QDir dir(path);

        if (dir.exists()) {
            qInfo() << "[FileService] Directory already exists:" << path;
            return true;
        }

        if (!dir.mkpath(".")) {
            qWarning() << "[FileService] Failed to create directory:" << path;
            return false;
        }

        qInfo() << "[FileService] Directory created:" << path;

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
