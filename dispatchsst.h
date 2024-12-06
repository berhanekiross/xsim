#ifndef DISPATCHSST_H
#define DISPATCHSST_H

#include <xsim_config>
#include <list>

#include "dispatch.h"

namespace xsim {

class Node;
class Entity;

/**
 * @brief Sort block list in shortest setup time order.
 */
class XSIM_EXPORT DispatchSst : public Dispatch {
 public:
     /** @brief Default constructor */
     DispatchSst();

     /* Documented in dispatch.h */
     void sort(Node *node, std::list<Entity*> *block_list) override;

 private:
     /**
      * @brief Helper class for sorting.
      */
     class SstSorter {
      public:
          SstSorter(Node *node) : node_(node) {}
          bool operator() (const Entity *mu1,
                  const Entity *mu2) const;
      private:
          Node *node_;
     };
};

} // namespace xsim

#endif // DISPATCHSST_H
