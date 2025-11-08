#ifndef I_API_CONTROLLER_H
#define I_API_CONTROLLER_H


/**
 * @brief Interface for API controllers handling data fetching operations.
 * @tparam TRequest Type of the request data, must be movable and default constructible.
 */
template<typename TRequest>
class IApiController
{
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~IApiController() = default;

    /**
     * @brief Fetch data by api call.
     * @param request - Requests parameters for the API call.
     */
    virtual void fetchData(TRequest request) = 0;
};

#endif // I_API_CONTROLLER_H
