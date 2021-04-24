#ifndef ATTRIBUTELISTIMPL_HPP
#define ATTRIBUTELISTIMPL_HPP

#include <xercesc/sax/AttributeList.hpp>

#include <map>
#include <memory>

#include "attributeimpl.hpp"

namespace XERCES_CPP_NAMESPACE {

class AttributeListImpl: public AttributeList
{
public:
    AttributeListImpl();

    XMLSize_t getLength() const override {
        return attributes.size();
    }

    const XMLCh *getName(const XMLSize_t index) const override {
        const AttributeImpl *a{ai(index)};
        return a ? a->name() : nullptr;
    }

    const XMLCh *getType(const XMLSize_t index) const override {
        const AttributeImpl *a{ai(index)};
        return a ? a->type() : nullptr;
    }

    const XMLCh *getValue(const XMLSize_t index) const override {
        const AttributeImpl *a{ai(index)};
        return a ? a->value() : nullptr;
    }

    const XMLCh *getType(const XMLCh *name) const override {
        auto iter{attributes.find(StrX(name).localForm())};
        return ((iter != attributes.end()) ? iter->second.get()->type() : nullptr);
    }

    const XMLCh *getValue(const XMLCh *name) const override {
        return getValue(StrX(name).localForm());
    }

    const XMLCh *getValue(const char *name) const override {
        auto iter{attributes.find(name)};
        return ((iter != attributes.end()) ? iter->second.get()->value() : nullptr);
    }

    typedef std::shared_ptr<AttributeImpl> attribute_ptr_t;
    void add(const char *name, const char *value) {
        attributes.emplace(name, attribute_ptr_t(new AttributeImpl(name, value)));
    }

private:
    typedef std::map<std::string, attribute_ptr_t> attribute_map_t;

    const AttributeImpl *ai(XMLSize_t index) const {
        attribute_map_t::const_iterator iter{attributes.begin()};
        std::advance(iter, index);
        return (iter != attributes.end()) ? iter->second.get() : nullptr;
    }

    attribute_map_t attributes;
};

} // end namespace XERCES_CPP_NAMESPACE //

#endif // ATTRIBUTELISTIMPL_HPP
