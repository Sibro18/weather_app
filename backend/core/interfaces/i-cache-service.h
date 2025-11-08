#ifndef I_CACHE_SERVICE_H
#define I_CACHE_SERVICE_H


/**
 * @brief The ICacheService class
 */
template<typename T>
class ICacheService
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~ICacheService() = default;

    /**
     * @brief Retrieve value from cache by key.
     * @param key - The key to look up.
     * @return Optional containing value if found, empty otherwise.
     *
     */
    virtual std::optional<T> get(const QString& key) = 0;

    /**
     * @brief Store value in cache with specified key.
     * @param key - The key for storage.
     * @param data - The data to cache.
     */
    virtual void set(const QString& key, T data) = 0;

    /**
     * @brief Remove value from cache by key.
     * @param key - The key to remove.
     */
    virtual void remove(const QString& key) = 0;

    /**
     * @brief Clear all cached data.
     */
    virtual void clear() = 0;

    /**
     * @brief Check if key exists in cache.
     * @param key - The key to check.
     * @return True if key exists, false otherwise.
     */
    virtual bool contains(const QString& key) = 0;
};

#endif // I_CACHE_SERVICE_H
