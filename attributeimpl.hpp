#ifndef ATTRIBUTEIMPL_HPP
#define ATTRIBUTEIMPL_HPP

#include "SAXPrint.hpp"

namespace XERCES_CPP_NAMESPACE {

class AttributeImpl
{
public:
    AttributeImpl(const AttributeImpl &other) = delete;
    AttributeImpl(AttributeImpl &&other) = delete;

    AttributeImpl(const char *name, const char *value, const char *type = "CDATA"):
        m_name{name}, m_type{type}, m_value{value}
    {}

    const XMLCh *name() const { return m_name.unicodeForm(); }
    const XMLCh *type() const { return m_type.unicodeForm(); }
    const XMLCh *value() const { return m_value.unicodeForm(); }

private:
    const XStr m_name;
    const XStr m_type;
    const XStr m_value;
};

} // end namespace XERCES_CPP_NAMESPACE //

#endif // ATTRIBUTEIMPL_HPP
