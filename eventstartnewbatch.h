#ifndef EVENTSTARTNEWBATCH_H
#define EVENTSTARTNEWBATCH_H

#include <xsim_config>

#include "event.h"

namespace xsim {

class Batch;

class XSIM_EXPORT EventStartNewBatch : public Event {
 public:
     EventStartNewBatch(Batch *batch,
             int priority = PRIORITY_STARTNEWBATCH);

     /* Documented in event.h */
     void process() override;
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Batch *batch_;
};

} // namespace xsim

#endif
