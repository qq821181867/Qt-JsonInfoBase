#include "JsonInfoBase.h"
#include <QDebug>
#include <QMetaProperty>
bool JsonInfoBase::fromJson(const QJsonObject &obj)
{
    QList<QByteArray> listPropertyNames = getPropertyNames();

    foreach (const QByteArray &name, listPropertyNames)
    {
        if(!obj.contains(name))
        {
            qDebug()<<__FUNCTION__<<"error:"<<name;
            continue;
        }
        setProperty(name,obj.value(name));
    }

    return true;
}

QJsonObject JsonInfoBase::toJson() const
{
    QJsonObject obj;
    //
    QList<QByteArray> listPropertyNames = getPropertyNames();
    //
    foreach (const QByteArray &name, listPropertyNames)
    {
        if(!property(name).isValid() || property(name).isNull())
        {
            continue;
        }
        obj.insert(name,propertyDataToJsonValue(property(name)));
    }
    //
    return obj;
}

QJsonObject JsonInfoBase::toJsonInSections(const QList<QByteArray> &sections) const
{
    QJsonObject obj;
    //
    foreach (const QByteArray &name, sections)
    {
        if(!property(name).isValid() || property(name).isNull())
        {
            continue;
        }
        obj.insert(name,propertyDataToJsonValue(property(name)));
    }
    //
    return obj;
}

QJsonObject JsonInfoBase::toJsonOutSections(const QList<QByteArray> &sections) const
{
    QJsonObject obj;
    //
    QList<QByteArray> listPropertyNames = getPropertyNames();
    foreach (auto section, sections)
    {
        listPropertyNames.removeAll(section);
    }
    //
    foreach (const QByteArray &name, listPropertyNames)
    {
        if(!property(name).isValid() || property(name).isNull())
        {
            continue;
        }
        obj.insert(name,propertyDataToJsonValue(property(name)));
    }
    //
//    qDebug()<<__FUNCTION__<<obj;
    return obj;
}

QList<QByteArray> JsonInfoBase::getPropertyNames() const
{
    QList<QByteArray> listPropertyNames;
    auto windowMetaObject = this->metaObject();
    for (int i=0; i < windowMetaObject->propertyCount(); i++)
    {
        listPropertyNames<<windowMetaObject->property(i).name();
    }
    listPropertyNames.removeAll("objectName");//移除自带的
    return listPropertyNames;
}

QJsonValue JsonInfoBase::propertyDataToJsonValue(const QVariant &data) const
{
    if(data.type()==QMetaType::QJsonObject)
    {
        return data.toJsonObject();
    }
    else if(data.type()==QMetaType::QJsonValue)
    {
        return data.toJsonValue();
    }
    else if(data.type()==QMetaType::QJsonArray)
    {
        return data.toJsonArray();
    }
    return QJsonValue::fromVariant(data);
}
