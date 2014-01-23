#ifndef OUTPUTELEMENT_H
#define OUTPUTELEMENT_H
#include "../ioelement.h"
namespace motorcar {
class OutputElement : public IOElement
{
public:
    OutputElement();
    bool isInputElement() const;
};
}


#endif // OUTPUTELEMENT_H
