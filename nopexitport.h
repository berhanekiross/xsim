#ifndef NOPEXITPORT_H
#define NOPEXITPORT_H

#include <xsim_config>

#include "exitport.h"

namespace xsim {

class Node;

/**
 * @brief A departure that does not support or allow successor nodes.
 */
class XSIM_EXPORT NopExitPort : public ExitPort {
 public:
     NopExitPort(Node *node);
};

} // namespace xsim

#endif // NOPEXITPORT_H
