#ifndef LOGICRESOURCE_H
#define LOGICRESOURCE_H

#include <xsim_config>
#include <map>
#include <string>

#include "node.h"

namespace xsim {

class Event;
class NodeResource;
class LogicSkill;
class NodeResource;
class NodeSkill;
class ResourceManager;

/**
 * @brief A node that provide services to other nodes.
 */
class XSIM_EXPORT LogicResource : public Node {
 public:
     /**
      * @brief Constructor.
      *
      * @param resource_manager The resource manager this resource belongs to.
      */
     LogicResource(ResourceManager *resource_manager);

     /**
      * @brief Destructor.
      */
     virtual ~LogicResource();

     void init() override;
     void finalize() override;
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
     size_t max_occupation() const override;
     size_t content_size() const override;

	 /**
	  * @brief Should be called once the task the resource
	  *			is assigned to begins, e.g. once an out event
	  *			is sheduled for an operation.
	  *			
	  *			Used to trigger the working state of the resource.
	  */
	 void work_started();

	 /**
	  * @brief Should be called once the task the resource
	  *			is assigned to is interrupted, e.g. when
	  *			task processing is interrupted by a failure.
	  *			
	  *			Used to trigger the waiting state of the resource.
	  */
	 void wait_started();

     void add_node_skill(NodeSkill *skill);
     void add_skill(LogicSkill *skill);
     size_t skills_count() const;
     double execution_factor() const;
     //bool have_skill(Skill *skill) const;
     LogicSkill* get_skill(int id) const;
     //void allocate();
     void release();
     void set_node_resource(NodeResource *node_resource);
     NodeResource* node_resource() const;
     void interrupt();
     void set_resource_type(ResourceType type);
	 /**
	  * @brief Set ready state of the resource. Default is True while False
	  *			is used during the delay caused by a response time. An event
	  *			when the resource will be ready is required when set to False.
	  *
	  * @Param value		Desired ready value, true or false.
	  * @Param ready_event	0 when value=true, otherwise an event when
	  *						the resource will be ready again.
	  */
     void set_ready(bool value, Event *ready_event);
     bool ready() const;
	 /**
	  * @Brief Getter for ready event cancelled.
	  *
	  * @Return True if there exist a cancelled ready event, otherwise false.
	  */
	 bool ready_event_cancelled() const;
	 /**
	  * @Brief Used to cancel possibly scheduled ready event and log
	  *			remaining time until the resource will be ready.
	  */
	 void try_cancel_ready_event();
	 /**
	  * @Brief Used to reschedule any cancelled ready event.
	  */
	 void try_reschedule_ready_event();
	 /**
	  * @Brief Used to completely remove an active ready event,
	  *			i.e. event will be removed from the event list
	  *			and deleted.
	  */
	 void reset_ready();

 private:
     void interrupt_();
     void resume_();
     NodeResource *node_resource_;
     ResourceManager *resource_manager_;
     std::map<int, LogicSkill*> skills_;
     std::vector<NodeSkill*> node_skills_;
     double execution_factor_;
     int ref_;
     bool ready_;
     ResourceType resource_type_;
	 Event *ready_event_;
	 bool ready_event_cancelled_;
	 simtime remaining_ready_time_;
};

} // namespace xsim

#endif // LOGICRESOURCE_H
