#ifndef EVENTDISRUPTIONBEGIN_H
#define EVENTDISRUPTIONBEGIN_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class Failure;
class Node;

class XSIM_EXPORT EventDisruptionBegin : public Event {
 public:
     EventDisruptionBegin(Node *node, Failure *failure,
             int priority = PRIORITY_DISRUPTION_BEGIN);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Node *node_;
     Failure *failure_;
};

} // namespace xsim

#endif // EVENTDISRUPTIONBEGIN_H
