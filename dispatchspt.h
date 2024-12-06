#ifndef DISPATCHSPT_H
#define DISPATCHSPT_H

#include <xsim_config>
#include <list>

#include "dispatch.h"

namespace xsim {

class Node;
class Entity;

/**
 * @brief Sort the block list in shortest processing time order.
 */
class XSIM_EXPORT DispatchSpt : public Dispatch {
 public:
     /** @brief Default constructor */
     DispatchSpt();

     /* Documented in distpatch.h */
     void sort(Node *node, std::list<Entity*> *block_list) override;

 private:
     /**
      * @brief Helper class for sorting
      */
     class SptSorter {
      public:
          SptSorter(Node *node) : node_(node) {}
          bool operator() (const Entity *mu1,
                  const Entity *mu2) const;
      private:
          Node *node_;
     };
};

} // namespace xsim

#endif // DISPATCHSPT_H
