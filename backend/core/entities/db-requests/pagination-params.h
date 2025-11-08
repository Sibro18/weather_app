#ifndef PAGINATION_PARAMS_H
#define PAGINATION_PARAMS_H

/**
 * @brief Parameters for pagination control.
 */
struct PaginationParams
{
    /**
     * @brief Current page number.
     */
    int page;

    /**
     * @brief Number of items per page.
     */
    int limit;
};

#endif // PAGINATION_PARAMS_H
