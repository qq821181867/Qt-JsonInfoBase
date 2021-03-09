# Qt-JsonInfoBase
快速封装一个可以直接转QJsonObject的类

利用Q_PROPERTY自定义属性性质，可进行统一封装转QJsonObject的类

原理：

1.类似

class testObj : QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString name MEMBER m_name)
    m_name;
}
这样我们就声明了一个name属性，类型为QString，并且绑定了私有成员m_name;

2.这时候我们通过继承的QObject中metaObject()函数获取对应元对象;

3.元对象中包含新增的自定义信息(propertyCount(),property(i).name()),继而得到所有的属性名称

4.在配合setProperty及property()函数即可做toJson()或fromJson()封装，fromJson()如下：

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
相对应的toJson也可以做类似的封装;

5.为了方便可将声明自定义属性及声明私有成员变量做一个宏，增加使用便捷性，如下：

#define ADDMEMBER(TYPE,NAME,DEFULT) Q_PROPERTY(TYPE NAME MEMBER m_##NAME) \
                             TYPE m_##NAME=DEFULT
//登录
class JsonInfoLoginSend : public JsonInfoBase
{
    Q_OBJECT
public:
    ADDMEMBER(QString,userName,"");
    ADDMEMBER(QString,password,"");
};
接下来使用仅需对JsonInfoLoginSend的m_userName、m_password赋值即可直接通过toJson()获取QJsonObject对象,当然也可以通过转json包直接fromJson得到成员变量对应赋值。


