#ifndef EVENTBATCHTIMEOUT_H
#define EVENTBATCHTIMEOUT_H

#include <xsim_config>

#include "event.h"

namespace xsim {

class Batch;

/**
 * @brief Event used to force a 'early' finish of an incomplete batch
 * when the 'start_incomplete' is true. A user specified timeout will
 * determine the time of the event, i.e. how long time the batch logic
 * will wait for a suitable entity for the batch to arrive before the
 * batch is finished although it is not completed.
 */
class XSIM_EXPORT EventBatchTimeOut : public Event {
 public:
     /**
      * @brief  Constructor.
      *        
      * @note   PRIORITY_OUT + 1 is used to make sure that out events
      * that could provide matching entities for the batch occur before
      * this event.
      * 
      * @param  batch    The batch logic that created the event.
      * @param  batch_if The id of the batch that should be finised.
      */
     EventBatchTimeOut(Batch *batch, unsigned int batch_id,
             int priority = PRIORITY_OUT + 1);

     /**
      * @brief Used to trigger the an 'early' finish of a specific
      * batch (batch_id_) of the batch logic (batch_).
      */
     void process() override;
     /* Documented in event.h */
     std::string sender() override;
     std::string receiver() override;
     std::string name() override;

 private:
     Batch *batch_;
     unsigned int batch_id_;
};

} // namespace xsim

#endif
