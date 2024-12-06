#ifndef DISPATCHORDER_H
#define DISPATCHORDER_H

#include <xsim_config>
#include <list>

#include "dispatch.h"

namespace xsim {

class Node;
class Entity;

/**
 * @brief Sort block list based on order priority.
 */
class XSIM_EXPORT DispatchOrder : public Dispatch {
 public:
     /** @brief Default constructor */
     DispatchOrder();

     /* Documented in dispatch.h */
     void sort(Node *node, std::list<Entity*> *block_list) override;

 private:
     /**
      * @brief Helper class for sorting.
      */
     class OrderSorter {
      public:
          OrderSorter(Node *node) : node_(node) {}
          bool operator() (const Entity *mu1,
                  const Entity *mu2) const;
      private:
          /**
          * @brief The node that dispatchs the entities.
          */
          Node *node_;
     };
};

} // namespace xsim

#endif // DISPATCHORDER_H
