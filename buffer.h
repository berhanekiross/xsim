#ifndef BUFFER_H
#define BUFFER_H

#include <xsim_config>
#include <string>
#include <deque>

#include "entitytime.h"
#include "int.h"
#include "node.h"

namespace xsim {

class EventOut;
class Failure;
class Entity;
class Simulation;
class NumberGenerator;

/**
 * @brief A node that can store one or more entities. The entity
 * leave in first in first out order.
 */
class XSIM_EXPORT Buffer : public Node {
 public:
     /**
      * @brief Constructor.
      */
     Buffer();

     void finalize() override;
     void init() override;
     void reset_stats() override;

     /* Documented in node.h */
     void define_outputs() override;
     void disruption_begin(Failure *failure,
            std::map<Node*, bool> &visited, int level,
            bool propagate_failure = true) override;
     void disruption_end(Failure *failure,
            std::map<Node*, bool> &visited, int level,
            bool propagate_failure = true) override;
     void unplanned_begin() override;
     void unplanned_end() override;
     void paused_begin() override;
     void paused_end() override;
     void interrupt_processing_resource() override;
     void resume_processing_resource(bool add_elapsed_time = true) override;
     void interrupt_repair_resource() override;
     void resume_repair_resource(bool add_elapsed_time = true) override;
     void interrupt_setup_resource() override;
     void resume_setup_resource(Entity *entity, bool add_elapsed_time = true) override;
     double relative_occupation() const override;
     size_t max_occupation() const override;
     size_t min_occupation() const override;
     size_t content_size() const override;
     std::string text() const override;
     bool enter(Entity* entity, Node* departure) override;
     void leave(Entity *entity, Node *node) override;

     /**
      * @brief Sets maximum buffer size
      *
      * @param max_size The maximum number of entities that can be stored at any given time.
      */
     void set_max_size(Int max_size);

     /**
      * @brief Get the maximum size of this buffer.
      *
      * @return The maximum size.
      */
     Int max_size() const;

     /**
      * @brief Get a all entities that are currently located on this
      * buffer, along with the simulation time on which they can leave.
      *
      * @return All entities on this buffer.
      */
     std::deque<EntityTime> buffer_contents() const;

     /**
      * @brief Check if the buffer is at full capacity.
      *
      * @return True if it is at full capacity.
      */
     bool is_full() const;

 private:
     /**
      * @brief All entities that currently are located on this node, along
      * with the simulation time they are ready to leave.
      */
     std::deque<EntityTime> buffer_;

     /**
      * @brief The maximum capacity of entities on this node.
      */
     Int max_size_;

     /**
      * @brief The maximum number of simultaneous entities on this buffer.
      */
     size_t max_occupied_;

     /**
      * @brief The minimum number of simultaneous entities on this buffer.
      */
     size_t min_occupied_;

     /**
      * @brief The simulation time when stats were last saved.
      */
     simtime enter_time_;
};

} // namespace xsim

#endif // BUFFER_H
