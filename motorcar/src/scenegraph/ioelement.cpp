#include "ioelement.h"
using namespace motorcar;

IOElement::IOElement()
{
}

bool IOElement::isOutputElement() const
{
    return !isInputElement();
}
