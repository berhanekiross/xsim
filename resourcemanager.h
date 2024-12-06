#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <xsim_config>
#include <string>
#include <vector>
#include <map>

#include "node.h"

namespace xsim {

class ExitPort;
class LogicResource;
class Entity;
class NodeResource;
class NodeSkill;
class NumberGenerator;

struct ResourceSkill {
    std::string name;
    double execution_factor;
};

struct Resource {
    std::string name;
    bool active;
    int amount;
    std::vector<ResourceSkill> skills;
};

/**
 * @brief Manages a set of resources.
 */
class XSIM_EXPORT ResourceManager : public Node {
 public:
     class BlockListItem {
     public:
         BlockListItem(NodeResource *r, Entity *e, Node *n, Failure *f, ResourceType type) :
             resource(r),
             entity(e),
             node(n),
             failure(f),
             type_(type)
         { }
         NodeResource *resource;
         Entity *entity;
         Node *node;
         Failure *failure;
         ResourceType type_;
     };

     /**
      * @brief Constructor.
      */
     ResourceManager();

     /**
      * @brief Destructor.
      */
     virtual ~ResourceManager();

     void init() override;
     void finalize() override;
     void reset_stats() override;
     void add_failure(Failure* failure) override;

     /* Documented in node.h */
     void unplanned_begin() override;
     void unplanned_end() override;
     void paused_begin() override;
     void paused_end() override;
     size_t max_occupation() const override;
     size_t content_size() const override;

     /** @brief Prepare for simulation by creating the resource objects. */
     virtual void pre_simulation_init();

     void add_resource(Resource *resource);
     const std::vector<Resource*>& resources() const;

     void add_logic_resource(LogicResource *resource);
     //Resource* request_processing_resource(Skill *skill);
     bool allocate_resources(NodeResource *node_resource, bool interrupt, Entity *entity, Failure *failure, ResourceType type);
     //bool interrupt_processing_resources(NodeResource *node_resource, Entity *entity, Failure *failure, ResourceType type);
     void add_to_block_list(NodeResource *resource, Entity *entity, Node *node, Failure *failure, ResourceType type);
     void remove_from_block_list(NodeResource *resource, Entity *entity);
     void set_response_time(NumberGenerator *response_time_generator);
     NumberGenerator* response_time() const;
     void trigger_blocklist();

	 std::list<LogicResource*> all_resources() const;

     void define_outputs() override;
     void set_outputs() override;

 private:
     typedef std::list<LogicResource*>::iterator resource_iter;

     /**
      * @brief Gets the average of a specific output from all operations.
      *
      * @param name The name of the output to get the average of.
      *
      * @returns The average value.
      */
     double average_output(const std::string &name) const;

     std::vector<Node*> nodes_;
     std::vector<Resource*> resources_;
     std::list<LogicResource*> logic_resources_;
     std::list<LogicResource*> all_logic_resources_;
     std::list<LogicResource*> allocated_;
     std::list<BlockListItem> block_list_;
     NumberGenerator *response_time_;
     //std::list<std::pair<EnterPort*, Skill*> > block_list_;
     //std::map<Skill*, std::list<Node*> > block_list_;
};

} // namespace xsim

#endif // OPERATION_H
