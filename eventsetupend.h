#ifndef EVENTSETUPEND_H
#define EVENTSETUPEND_H

#include <xsim_config>
#include <vector>

#include "event.h"

namespace xsim {

class EnterPort;

class XSIM_EXPORT EventSetupEnd : public Event {
 public:
     EventSetupEnd(Node *node,
             int priority = PRIORITY_SETUP);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Node *node_;
};

} // namespace xsim

#endif // EVENTSETUPEND_H
