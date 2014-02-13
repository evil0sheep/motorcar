#ifndef INPUTELEMENT_H
#define INPUTELEMENT_H

#include "../ioelement.h"
namespace motorcar {

class InputElement : public IOElement
{
public:
    InputElement();
    virtual ~InputElement() {}

    bool isInputElement() const;
};

}


#endif // INPUTELEMENT_H
