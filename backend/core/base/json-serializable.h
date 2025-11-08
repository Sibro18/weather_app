#ifndef JSON_SERIALIZABLE_H
#define JSON_SERIALIZABLE_H


#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>


/**
 * @brief A CRTP interface for JSON serialization/deserialization.
 * @tparam T - The derived class type (using CRTP pattern).
 */
template<typename T>
class JsonSerializable
{
public:
    virtual ~JsonSerializable() = default;

    /**
     * @brief Convert the object to a JSON representation.
     * @return QJsonObject containing the object's data.
     */
    virtual QJsonObject toJson() const = 0;

    /**
     * @brief Populate the object from JSON data.
     * @param obj JSON object containing the data to populate from.
     */
    virtual void fillFromJsonObject(const QJsonObject& obj) = 0;

    /**
     * @brief Convert a list of objects to a JSON array.
     * @param items - List of objects to convert.
     * @return QJsonArray containing the serialized objects.
     */
    inline static QJsonArray toJsonArray(const QList<T>& items)
    {
        QJsonArray array;

        for (const T& item : items)
        {
            array.append(item.toJson());
        }

        return array;
    }

    /**
     * @brief Convert a JSON array to a list of objects.
     * @param jsonArray - JSON array to convert from.
     * @return QList<T> containing the deserialized objects.
     */
    inline static QList<T> fromJsonArray(const QJsonArray& jsonArray)
    {
        QList<T> result;
        result.reserve(jsonArray.size());

        for (const QJsonValue& value : jsonArray)
        {
            if (value.isObject())
            {
                T item;
                item.fillFromJsonObject(value.toObject());

                result.append(std::move(item));
            }
        }

        return result;
    }

    /**
     * @brief Convert the object to a JSON string.
     * @param compact - If true, uses compact formatting; otherwise uses indented formatting.
     * @return QString containing the JSON representation.
     */
    inline QString toJsonString(bool compact = true) const
    {
        QJsonDocument doc(toJson());

        return doc.toJson(compact
            ? QJsonDocument::Compact
            : QJsonDocument::Indented
        );
    }

    /**
     * @brief Populate the object from a JSON string.
     * @param jsonString - JSON string to parse.
     * @return QJsonParseError - parseError.error != QJsonParseError::NoError if success filling.
     */
    inline QJsonParseError fillFromJsonString(const QString& jsonString)
    {
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(
            jsonString.toUtf8(),
            &parseError
        );

        if (parseError.error != QJsonParseError::NoError && doc.isObject())
        {
            fillFromJsonObject(doc.object());
        }

        return parseError;
    }
};

#endif // JSON_SERIALIZABLE_H
