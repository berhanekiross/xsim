#ifndef TAKT_H
#define TAKT_H

#include <xsim_config>
#include <string>
#include <vector>

#include "enterlogic.h"
#include "exitlogic.h"
#include "simulation.h"
#include "double.h"

namespace xsim {

class EventTaktComplete;
class Failure;
class Node;

/**
 * @brief Moves entities in sync between nodes.
 */
class XSIM_EXPORT Takt : public ExitLogic, public EnterLogic {
 public:
     /**
      * @brief Constructor.
      */
     Takt();

     /* Documented in object.h */
     void simulation_init() override;
     void init() override;
     void finalize() override;
     void reset_stats() override;
     void define_outputs() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /* Documented in exitlogic.h */
     bool allow_leave(Node *node, Entity *entity) override;

     /**
      * @param takt_time Sets the minimum time for the takt.
      */
     void set_takt_time(Double takt_time);

     /**
      * @returns The takt time.
      */
     Double takt_time() const;

     /**
      * @brief Add a node that should be part of this takt.
      *
      * @param node The departure node.
      */
     void add_node(Node *node);

     /**
      * @brief Set which node is the first in takt.
      *
      * @param node The node.
      */
     void set_first_node(Node *node);

     /**
      * @brief Try to move all entities one step.
      */
     void try_takt_move();

     /**
      * @brief Do the necessary actions when a entity enters the takt.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entering(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Do the necessary actions when a entity exits the takt.
      *
      * @param  departure   The node the entity exited.
      * @param  destination The node the entity entered.
      * @param  entity      The entity that exited.
      */
     void exiting(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Report that a node has finished processing its entity.
      *
      * @param node The node that is finished.
      */
     void update_finished(Node *node);

     /**
      * @brief Report that a node has finished processing its entity.
      *
      * @param  node    The node that is finished.
      * @param  failure The failure that ended.
      */
     void update_finished_disruption_ended(Node* node, Failure* failure);

 private:
     /**
      * @brief Schedule a takt out.
      */
     void schedule_new_takt_out();

     /**
      * @brief Schedule a blocked takt out.
      */
     void schedule_blocked_takt_out();

     /**
      * @brief Check if there is any entity in this takt.
      *
      * @return True if it is emtpy.
      */
     bool is_takt_empty() const;

     /**
      * @brief Check if a node have finished processing its entity.
      *
      * @param node The node to check against.
      *
      * @return True if it is finished.
      */
     bool is_done_processing(Node *node) const;

     /**
      * @brief Check if there is a entity ready and waiting at the
      * entrance.
      *
      * @return True if a entity is ready to enter.
      */
     bool is_entity_ready_at_entrance() const;

     /**
      * @brief Collect statistics.
      */
     void save_stats();

     /**
      * @brief Count the length of the chain of successors from a given node.
      * When a node that is not part of this takt is encountered the chain is
      * stopped.
      *
      * @return The length of the successor chain.
      */
     int get_successors_chain_length(Node *node) const;

     /**
      * @brief Sort nodes by successor chain length.
      *
      * @param nodes The nodes that should be sorted.
      */
     void sort_nodes(std::vector<Node*> &nodes) const;

     /**
      * @brief The nodes that follow this takt.
      */
     std::vector<Node*> nodes_;

     /**
      * @brief The first node in the takt.
      */
     Node *first_node_;

     /**
      * @brief The minimum amount of time one takt takes.
      */
     Double takt_time_;

     /**
      * @brief True if a move is allowed.
      */
     bool move_allowed_;

     /**
      * @brief True if the takt is blocked.
      */
     bool blocked_;

     /**
      * @brief Register over which nodes that have finished processing its
      * entity.
      */
     std::map<Node*, simtime> finished_;

     /**
      * @brief Register over which entity that are allowed to move.
      */
     std::map<Entity*, bool> allowed_to_move_;

     /**
      * @brief The takt complete event.
      */
     EventTaktComplete *event_takt_complete_;

     /**
      * @brief The simulation time of the next scheduled takt out.
      */
     simtime takt_out_time_;

     /**
      * @brief The simulation time of the last complete takt.
      */
     simtime takt_complete_time_;

     /**
      * @brief The total amount of time that exceeded the takt time.
      */
     simtime exceed_time_;

     /**
      * @brief The total amount of time that deceeded the takt time.
      */
     simtime deceed_time_;

     /**
      * @brief Exceed times per node.
      */
     std::map<Node*, simtime> exceed_times_;

     /**
      * @brief Deceed times per node.
      */
     std::map<Node*, simtime> deceed_times_;

     /**
      * @brief Count of how many takts each node have been active, if they
      * are processing a entity they are regarded as active.
      */
     std::map<Node*, unsigned int> active_counts_;

     /**
      * @brief Count of how many takts each node have been inactive, if they
      * are not processing a entity they are regarded as inactive.
      */
     std::map<Node*, unsigned int> inactive_counts_;

     /**
      * @brief The number of complete takts.
      */
     unsigned int num_takts_;

     /**
      * @brief The total number of time the takt have exceed the takt time.
      */
     unsigned int num_exceed_total_;

     /**
      * @brief The number of time each node have exceed the takt time.
      */
     std::map<Node*, unsigned int> num_exceed_;

     /**
      * @brief The number of time each node have deceed the takt time.
      */
     std::map<Node*, unsigned int> num_deceed_;

     /**
      * @brief Count of how many entities that are currently in this takt.
      */
     unsigned int num_entities_in_takt_;
};

} // namespace xsim

#endif // TAKT_H
