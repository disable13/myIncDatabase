#include "dxml.h"
//
#include "../../3rdparty/tinyxml/tinyxml.h"
//
DXml::DXml(QObject* parent/* = NULL*/)
    : QObject(parent), m_Doc(NULL), m_CurrentElement(NULL),
      m_ParentElement(NULL), m_Open(false)
{ }
//
DXml::~DXml()
{
    FREE_MEM(m_CurrentElement);
    FREE_MEM(m_ParentElement);
    FREE_MEM(m_Doc);
}
//
bool DXml::open(const QString &fileName)
{
    if(m_Open)
        return false;

    m_Doc = new TiXmlDocument(qStringConst(fileName));
    if(!m_Doc->LoadFile()) {
        FREE_MEM(m_Doc); /// FIXME: add error handle
        return false;
    }
    m_CurrentElement = m_Doc->FirstChildElement();

    return m_Open = true;
}
//
bool DXml::selectElement(const QString& elemPath)
{
    if (!m_Open)
        return false;
    if (elemPath.length() == 0)
        return false;
    if (m_CurrentElement == NULL)
        return false;

    const QString sElem = elemPath.section('.', 0, 0);
    if (QString(m_CurrentElement->Value()).toLower() != sElem.toLower()) {
        FREE_MEM(m_ParentElement);
        m_ParentElement = m_CurrentElement;
        TiXmlNode* node =
                m_ParentElement->FirstChild(qStringConst(sElem));//->ToElement();

        if (node == NULL)
            return false;
        m_CurrentElement = node->ToElement();
        FREE_MEM(node);
    }
    if (sElem.length() == elemPath.length())
        return true;
    else {
        QString arg = elemPath;
        arg.replace(sElem + ".", "");
        return selectElement(arg);
    }
}
//
bool DXml::selectNext()
{
    if(!m_Open)
        return false;
    if (m_CurrentElement == NULL)
        return false;
    FREE_MEM(m_ParentElement);

    m_ParentElement = m_CurrentElement;
    m_CurrentElement =
            m_ParentElement->NextSiblingElement();

    return m_CurrentElement != NULL;
}
//
bool DXml::selectParent()
{
    if(!m_Open)
        return false;
    if (m_CurrentElement == NULL)
        return false;
    FREE_MEM(m_ParentElement);
    m_ParentElement = m_CurrentElement;

    m_CurrentElement = m_ParentElement->Parent()->ToElement();

    return m_CurrentElement != NULL;
}
//
bool DXml::selectRoot()
{
    if(!m_Open)
        return false;
    if(m_Doc == NULL)
        return false;
    FREE_MEM(m_ParentElement);

    m_ParentElement = m_CurrentElement;
    m_CurrentElement = m_Doc->RootElement();

    return m_CurrentElement != NULL;
}

//
QString DXml::elementAttr(const QString &name) const
{
    if(m_CurrentElement == NULL)
        throw; // FIXME
    return m_CurrentElement->Attribute(qStringConst(name));
}
//
QString DXml::element() const
{
    if(m_CurrentElement == NULL)
        throw; // FIXME
    return m_CurrentElement->GetText();
}
//
quint32 DXml::countElements() const
{
    if(m_CurrentElement == NULL)
        throw; // FIXME

    quint32 p_nCount = 0;
    TiXmlNode* p_xnChild;
    for( p_xnChild = m_CurrentElement->FirstChild(); p_xnChild; p_xnChild = p_xnChild->NextSibling() )
        p_nCount++;
    FREE_MEM(p_xnChild);

    return p_nCount;
}
