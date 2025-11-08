#ifndef I_REPOSITORY_H
#define I_REPOSITORY_H


/**
 * @brief Generic repository interface for CRUD operations.
 * 
 * @tparam TEntity - The complete entity type returned by read operations.
 * @tparam TId - The type of entity identifier (int, QString, UUID, etc.).
 * @tparam TCreateDto -Data transfer object for create operations (defaults to TEntity).
 * @tparam TUpdateDto - Data transfer object for update operations (defaults to TEntity).
 * 
 * Provides basic Create, Read, Update, Delete operations for data persistence.
 */
template<
    typename TEntity, 
    typename TId, 
    typename TCreateDto = TEntity, 
    typename TUpdateDto = TEntity
>
class IRepository {
public:
    virtual ~IRepository() = default;
    
    /**
     * @brief Retrieve an entity by its unique identifier.
     * @param id - The unique identifier of the entity.
     * @return Entity if found, std::nullopt otherwise.
     */    
    virtual std::optional<TEntity> getById(TId id) = 0;

    /**
     * @brief Retrieve all entities with optional limit.
     * @param limit - Maximum number of entities to return (-1 for unlimited).
     * @return List of entities, possibly limited by count.
     */    
    virtual QList<TEntity> getAll(int limit = -1) = 0;

    /**
     * @brief Add a new entity to the repository.
     * @param entity - The entity to add.
     * @return True if entity was successfully added, false otherwise.
     */
    virtual bool add(TCreateDto entity) = 0;

    /**
     * @brief Update an existing entity in the repository.
     * @param entity - The entity with updated data.
     * @return True if entity was successfully updated, false otherwise.
     */
    virtual bool update(TUpdateDto entity) = 0;

    /**
     * @brief Remove an entity from the repository by its identifier.
     * @param id - The unique identifier of the entity to remove.
     * @return True if entity was successfully removed, false otherwise.
     */
    virtual bool remove(TId id) = 0;
};

#endif //I_REPOSITORY_H
