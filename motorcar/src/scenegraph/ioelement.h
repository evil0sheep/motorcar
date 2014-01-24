#ifndef IOELEMENT_H
#define IOELEMENT_H

#include "foo.h"

namespace motorcar {
class IOElement : public Foo
{
public:
    IOElement();
    virtual ~IOElement();

    //returns whether this is an input node or an outputNode
    //overidden in InputNode and OutputNode
    virtual bool isInputElement() const = 0;
    bool isOutputElement() const;
};
}


#endif // IOELEMENT_H
