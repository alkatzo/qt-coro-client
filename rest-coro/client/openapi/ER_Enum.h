/**
 * People API
 * An API that returns a list of people based on a provided date-time.
 *
 * The version of the OpenAPI document: 1.0.0
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#ifndef ER__ENUM_H
#define ER__ENUM_H

#include <QJsonValue>
#include <QMetaType>
#include <QString>

namespace er {

class ER_Enum {
public:
    ER_Enum() {}

    ER_Enum(QString jsonString) {
        fromJson(jsonString);
    }

    virtual ~ER_Enum() {}

    virtual QJsonValue asJsonValue() const {
        return QJsonValue(jstr);
    }

    virtual QString asJson() const {
        return jstr;
    }

    virtual void fromJson(QString jsonString) {
        jstr = jsonString;
    }

    virtual void fromJsonValue(QJsonValue jval) {
        jstr = jval.toString();
    }

    virtual bool isSet() const {
        return false;
    }

    virtual bool isValid() const {
        return true;
    }

private:
    QString jstr;
};

} // namespace er

Q_DECLARE_METATYPE(er::ER_Enum)

#endif // ER__ENUM_H
