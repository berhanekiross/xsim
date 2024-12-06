#ifndef EXITLOGIC_H
#define EXITLOGIC_H

#include <xsim_config>
#include <list>
#include <string>

#include "logic.h"

namespace xsim {

class Node;
class Simulation;

class XSIM_EXPORT ExitLogic : public virtual Logic {
 public:
     /**
      * @brief Constructor.
      */
     ExitLogic();

     void init() override;

     /**
      * @brief Check if a entity are allowed to exit a destination.
      *
      * @param node The node from which the entity wants to leave
      * @param entity The entity that wants to exit.
      *
      * @return True if it is allowed to exit.
      */
     virtual bool allow_leave(Node *node, Entity *entity) = 0;
};

} // namespace xsim

#endif // EXITLOGIC_H
