#ifndef FILE_SERVICE_H
#define FILE_SERVICE_H


#include <QByteArray>
#include <QSharedPointer>
#include <QMutex>


namespace GeneralUtils
{
    /**
     * @brief Thread-safe file system service.
     */
    class FileService : QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Default constructor.
         *
         * Initializes an empty FileService instance with no preloaded file mutexes.
         */
        explicit FileService(QObject* parent = nullptr);

        /**
         * @brief Destructor.
         *
         * Clears the internal mutex map. Ensures that all QSharedPointer<QMutex>
         * objects are released safely.
         */
        ~FileService();

        /**
         * @brief Saves multiple files atomically with thread-safety per file.
         *
         * Iterates over the provided map and writes each QByteArray to the
         * corresponding file path. If a file cannot be written or committed,
         * an error is logged. Each file operation is protected by its own mutex.
         *
         * @param fileNameDataMap - A map of file paths to byte array data to be saved.
         */
        void saveData(const QMap<QString, QByteArray>& fileNameDataMap);

        /**
         * @brief Reads the content of a file in a thread-safe manner.
         *
         * Locks a mutex for the specific file, opens it in read-only mode,
         * and returns its contents. If the file cannot be opened,
         * an empty QByteArray is returned.
         *
         * @param filePath - The full path of the file to read.
         * @return QByteArray containing the file's contents, or empty if an error occurs.
         */
        std::optional<QByteArray> getData(const QString& filePath);

        /**
         * @brief Reads the file names of directory.
         * @param dirPath - the relative or absolute path to directory.
         * @return QStringList with file names.
         */
        std::optional<QStringList> getFileNamesByDir(const QString& dirPath) const;

        /**
         * @brief Creates a directory if it does not exist.
         *
         * Uses QDir::mkpath() to ensure that the full directory path is created.
         * Logs success or failure.
         *
         * @param path - Directory path to create.
         * @return true if the directory exists or was created successfully, false otherwise.
         */
        bool createNewDirIfNotExists(const QString &path) const;

        /**
         * @brief Deleted copy constructor to prevent copying of the service.
         */
        FileService(const FileService&) = delete;

        /**
         * @brief Deleted copy assignment operator to prevent assignment of the service.
         */
        FileService& operator=(const FileService&) = delete;
    private:
        /**
         * @brief Map of file paths to their associated mutexes.
         *
         * Key: Cleaned file path string.
         * Value: Shared pointer to a QMutex guarding that file.
         */
        QMap<QString, QSharedPointer<QMutex>> _fileMutexMap;

        /**
         * @brief Mutex for controlling access to the mutex map itself.
         *
         * Prevents concurrent modifications to _fileMutexMap when creating
         * or retrieving file-specific mutexes.
         */
        QMutex _mapAccessMutex;

        /**
         * @brief Retrieves (or creates) a mutex for the specified file.
         *
         * Ensures that each file path is associated with a unique mutex to
         * coordinate concurrent access. Mutexes are stored in a shared pointer
         * so they can be reused safely across threads.
         *
         * @param fileName - The file path for which to retrieve a mutex.
         * @return Pointer to a QMutex guarding access to the specified file.
         */
        QMutex* _getMutexForFile(const QString& fileName);
    };
}
#endif // FILE_SERVICE_H
