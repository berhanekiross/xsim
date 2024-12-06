#ifndef EVENT_H
#define EVENT_H

#include <xsim_config>
#include <string>

#include "simulation.h"

namespace xsim {

/**
* @brief Base class for events.
*/
class XSIM_EXPORT Event {
 public:
     /**
      * @brief Constructor.
      */
     Event(int priority) : priority_(priority), sub_priority_(0), breakpoint_(false),
             breakpoint_stopped_(false), next_(0), prev_(0) {
         init(priority, 0);
     }

     Event(int priority, int sub_priority) : priority_(priority), sub_priority_(sub_priority), breakpoint_(false),
             breakpoint_stopped_(false), next_(0), prev_(0) {
         init(priority, sub_priority);
     }
    
     /**
      * @brief Destructor
      */
     virtual ~Event() {}

     /**
      * @brief Execute the event specific stuff.
      */
     virtual void process() = 0;

     /**
      * @brief Print any info about the event.
      */
     virtual void info() {}

     /**
      * @brief Check if the event has a breakpoint.
      *
      * @return True if there is a breakpoint.
      */
     bool breakpoint() const { return breakpoint_; }

     /**
      * @brief Set breakpoint on event.
      *
      * @param value True if the breakpoint should be active.
      */
     void set_breakpoint(bool value) { breakpoint_ = value; }

     /**
      * @brief Check if the simulation was stopped as a result of a breakpoint.
      *
      * @return True if the simulation stopped as a result of a breakpoint.
      */
     bool breakpoint_stopped() const { return breakpoint_stopped_; }

     /**
      * @brief Set if the simulation was stopped as a result of a breakpoint.
      */
     void set_breakpoint_stopped() { breakpoint_stopped_ = true; }

     /**
      * @brief Get the priority of the event.
      *
      * @return The priority.
      */
     int priority() const { return priority_; }

     /**
      * @brief Get the priority of the event.
      *
      * @return The priority.
      */
     int sub_priority() const { return sub_priority_; }

     /**
      * @brief Get the receiver of the event.
      *
      * @return The receiver.
      */
     virtual std::string receiver() { return ""; }

     /**
      * @brief Get the sender of the event.
      *
      * @return The sender.
      */
     virtual std::string sender() { return ""; }

     /**
      * @brief Get name of the event.
      *
      * @return The name of the event.
      */
     virtual std::string name() { return ""; }

     /**
      * @brief Get the next event.
      *
      * @return The next event.
      */
     Event* next() const { return next_; }

     /**
      * @brief Get the previous event.
      *
      * @return The previous event.
      */
     Event* prev() const { return prev_; }

     /**
      * @brief Set the next event.
      *
      * @param event The next event.
      */
     void set_next(Event *evt) { next_ = evt; }

     /**
      * @brief Set the previous event.
      *
      * @param event The previous event.
      */
     void set_prev(Event *evt) { prev_ = evt; }

     /**
      * @brief Set the time of the event.
      *
      * @param event The time when the event occurs.
      */
     void set_time(simtime time) { time_ = time; }

     /**
      * @brief Get the time of the event.
      *
      * @return The time when the event occurs.
      */
     simtime time() const { return time_; }

     /**
      * @brief Custom allocation operator.
      *
      * @param  size The size to allocate.
      */
     void* operator new(size_t size)
     {
         if (size <= sim()->allocator().chunk_size())
             return sim()->allocator().allocate(size);
         else
             return ::malloc(size);
     }

     /**
      * @brief Custom de-allocation operator.
      *
      * @param  ptr  The pointer that should be de-allocated.
      * @param  size The size.
      *
      * @returns The result of the operation.
      */
     void operator delete(void* ptr, size_t size)
     {
         if (size <= sim()->allocator().chunk_size())
             sim()->allocator().free(ptr);
         else
             ::free(ptr);
     }

 protected:
     /**
      * @brief Init the event.
      *
      * @param priority Set the priority of the event, this will affect the
      * order of events if they occur at the same simulation time. A Lower
      * priority event occurs before a higher priority event.
      */
     void init(int priority, int sub_priority)
     {
         priority_ = priority;
         sub_priority_ = sub_priority;
         breakpoint_ = false;
         next_ = 0;
         prev_ = 0;
     }
 private:
     /**
      * @brief The priority.
      */
     int priority_;

     /**
      * @brief The sub priority.
      */
     int sub_priority_;

     /**
      * @brief If true this event is not processed.
      */
     bool breakpoint_;

     /**
      * @brief True if the simulation stopped as a result of a breakpoint on
      * this event.
      */
     bool breakpoint_stopped_;

     /**
      * @brief The next event.
      */
     Event *next_;

     /**
      * @brief The previous event.
      */
     Event *prev_;

     /**
      * @brief The time of the event.
      */
     simtime time_;
};

} // namespace xsim

#endif // EVENT_H
