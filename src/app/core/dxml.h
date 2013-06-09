#ifndef DXML_H
#define DXML_H
//
#include <QObject>
//
#include "../core.h"
//
class TiXmlDocument;
class TiXmlElement;
//
class DXml : public QObject
{
    Q_OBJECT
public:
    DXml(QObject* parent = NULL);
    virtual ~DXml();

private:
    TiXmlDocument*  m_Doc;
    TiXmlElement*   m_CurrentElement;
    TiXmlElement*   m_ParentElement;
    //
    DQ_PROPERTY_R(bool, Open, private)

public:
    bool open(const QString &filename);
    //
    bool selectElement(const QString& elemPath);
    bool selectNext();
    bool selectParent();
    bool selectRoot();
    //
    QString elementAttr(const QString& name) const;
    QString element() const;
    quint32 countElements() const;

};
//
#endif // DXML_H
