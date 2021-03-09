#ifndef JSONINFOBASE_H
#define JSONINFOBASE_H

#include <QJsonObject>
#include <QObject>
#include <QPointer>
#include <QJsonArray>

#define ADDMEMBER(TYPE,NAME,DEFULT) Q_PROPERTY(TYPE NAME MEMBER m_##NAME) \
                             TYPE m_##NAME=DEFULT

class JsonInfoBase : public QObject
{
    Q_OBJECT
public:
    bool fromJson(const QJsonObject &obj);
    QJsonObject toJson() const;
    QJsonObject toJsonInSections(const QList<QByteArray> &sections) const;  //只转传入的部分变json
    QJsonObject toJsonOutSections(const QList<QByteArray> &sections) const; //传入的字段是不转json

protected:
    QList<QByteArray> getPropertyNames() const;
    QJsonValue propertyDataToJsonValue(const QVariant &data) const;
};

#endif // JSONINFOBASE_H
