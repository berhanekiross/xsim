#ifndef FAILUREZONE_H
#define FAILUREZONE_H

#include <xsim_config>
#include <string>

#include "node.h"
#include "int.h"

namespace xsim {

class EventDisruptionBegin;
class Failure;
class NumberGenerator;
class Simulation;

/**
 * @brief A node that process one movble unit at a time.
 */
class XSIM_EXPORT FailureZone : public Node {
 public:
     /**
      * @brief Constructor.
      * @param propagation_steps How many levels (steps) a failure should be
      * propagated.
      */
     FailureZone(Int propagation_steps);

     /* Documented in node.h */
     void disruption_begin(Failure *failure,
            std::map<Node*, bool> &visited, int level, 
            bool propagate_failure = true) override;
     void disruption_end(Failure *failure,
            std::map<Node*, bool> &visited, int level, 
            bool propagate_failure = true) override;
     size_t max_occupation() const override;
     size_t content_size() const override;

     /**
      * @returns The maximum number of propagation steps.
      */
     Int propagation_steps() const;

     /**
      * @brief Add a regular, non-failure zone, node that we should propagate
      * failures to.
      *
      * @param node The node to propagate failures to.
      */
     void add_node(Node *node);

     /**
      * @brief Add a failure zone that we should propagate failures to.
      *
      * @param node The failure zone to propagate failures to.
      */
     void add_failure_zone(Node *node);

 private:

     /**
      * @brief All non-failure zone nodes that we propagate failures to.
      */
     std::vector<Node*> nodes_;

     /**
      * @brief All failure zones that we propagate failures to.
      */
     std::vector<Node*> failure_zones_;

     /**
      * @brief The number of steps failures that originate in one of our
      * immidiate nodes should be propagated.
      */
     Int propagation_steps_;
};

} // namespace xsim

#endif // FAILUREZONE_H
