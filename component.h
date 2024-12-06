#ifndef COMPONENT_H
#define COMPONENT_H

#include <xsim_config>
#include <vector>

#include "object.h"

namespace xsim {

class Node;

/**
 * @brief A collection of objects.
 */
class XSIM_EXPORT Component : public Object {
public:
    /** @brief Default constructor */
    Component();
};

} // namespace xsim

#endif // COMPONENT_H