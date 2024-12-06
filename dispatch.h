#ifndef DISPATCH_H
#define DISPATCH_H

#include <xsim_config>
#include <list>

#include "object.h"

namespace xsim {

class Entity;
class Node;

/**
 * @brief Base class for sorting the block list in an arbitrary order.
 */
class XSIM_EXPORT Dispatch : public Object {
 public:
     /**
      * @brief Destructor
      */
     virtual ~Dispatch() {}
    
     /**
      * @brief Sort the block list in some arbitrary order.
      *
      * @param node The node that owns the block list.
      * @param block_list The block list that will have the new order.
      */
     virtual void sort(Node *node, std::list<Entity*> *block_list) = 0;
};

} // namespace xsim

#endif // DISPATCH_H
