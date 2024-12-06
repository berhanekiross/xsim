#ifndef NODERESOURCE_H
#define NODERESOURCE_H

#include <xsim_config>
#include <map>
#include <string>

#include "node.h"

namespace xsim {

class NodeSkill;

/**
 * @brief A node that provide services to other nodes.
 */
class XSIM_EXPORT NodeResource : public Node {
 public:
     enum Sorting {FEW_SKILLS_FAST_EXECUTION, FEW_SKILLS_SLOW_EXECUTION,
                   MANY_SKILLS_FAST_EXECUTION, MANY_SKILLS_SLOW_EXECUTION,
                   FAST_EXECUTION_FEW_SKILLS, FAST_EXECUTION_MANY_SKILLS,
                   SLOW_EXECUTION_FEW_SKILLS, SLOW_EXECUTION_MANY_SKILLS};
     /**
      * @brief Constructor.
      *
      * @param resource_manager The resource manager this resource belongs to.
      */
     NodeResource(
            Node *node,
            int priority,
            bool skills_first,
            bool interruptible,
            Sorting sorting);

     /**
      * @brief Destructor.
      */
     virtual ~NodeResource();

     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;

     /* Documented in node.h */
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
	  * @brief Forward working state change to used
	  *			logic resources.
	  */
	 void work_started();

	 /**
	  * @brief Forward waiting state change to used
	  *			logic resources.
	  */
	 void wait_started();

     int priority() const;
     bool skills_first() const;
     bool interruptible() const;
     void release_resources();
     void add_skill(NodeSkill *skill);
     size_t skills_count() const;
     Sorting sorting() const;
     double get_execution_factor() const;
     const std::vector<NodeSkill*>& skills() const;
     Node *node() const;
     /**
      * @brief Entity associated to the last attempt at allocating resources.
      *
      * Needed for skills first, e.g. for the case when resuming an
      * interrupted setup while the resource wasn't yet ready and no other
      * information about what entity caused the request for resources is
      * available.
      * 
      * @return The entity.
      */
     Entity *entity() const;
     /**
      * @brief Set entity to be associated with the last allocation of
      * resources.
      *
      * @param entity The entity to be set.
      */
     void set_entity(Entity *entity);
	 /**
	  * @brief Check if all required skills are allocated.
	  *			Note! All allocated resources need not be ready.
	  *
	  * @return True if all skills are allocated.
	  */
	 bool is_allocated() const;
	 /**
	  * @brief Check if all required skills are allocated and ready.
	  *
	  * @return True if all skills are allocated and ready.
	  */
	 bool is_allocated_and_ready() const;
	 /**
	  * @brief Check if any of the allocated resources
	  *			are interrupted (i.e. not operational).
	  *
	  * @return True if any allocated resource isn't
	  *			operational.
	  */
	 bool is_interrupted() const;

 private:
     Node *node_;
     Entity *entity_;
     std::vector<NodeSkill*> skills_;
     int priority_;
     bool skills_first_;
     bool interruptible_;
     Sorting sorting_;
};

} // namespace xsim

#endif // NODERESOURCE_H
