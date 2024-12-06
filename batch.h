#ifndef BATCH_H
#define BATCH_H

#include <xsim_config>
#include <map>
#include <vector>

#include "enterlogic.h"
#include "int.h"

namespace xsim {

class EventStartNewBatch;
class Node;
class NumberGenerator;
class Simulation;
class Store;
class ExitPort;
class Variant;

struct BatchItem {
    Variant* variant;
    Int size;
};

/**
* @brief Helper class to keep track of batches.
*/
class NodeItem {
 public:
     NodeItem(Variant *var = 0) :
             variant(var),
             assigned(0),
             started(0),
             completed(0)
     {
     }

     /**
      * @brief The type of variant in this batch.
      */
     Variant *variant;

     /**
      * @brief The number of entities currently assigned to this batch.
      */
     unsigned int assigned;

     /**
      * @brief The number of entities that have been started, entered but
      * not exited, in a batch.
      */
     unsigned int started;

     /**
      * @brief The number of entities that have exited the batch.
      */
     unsigned int completed;

     /**
      * @brief The total number of entities that should be produced in
      * each batch.
      */
     unsigned int size;
};

/**
 * @brief Group together entities in batches.
 */
class XSIM_EXPORT Batch : public EnterLogic {
 public:
     /**
      * @brief Constructor.
      */
     Batch();

     void init() override;
     void reset_stats() override;
     void finalize() override;
     void define_outputs() override;

     /* Documented in enterlogic.h */
     bool allow_enter(Node *node, Entity *entity) override;

     /**
      * @brief Update variant count for entered entity.
      *
      * @param node The node the entity wanted to enter.
      * @param entity The entity that was blocked.
      */
     void add_forward_blocking(Node *node, Entity *entity) override;

     /**
      * @param  value If true as many batches as possible are created,
      * otherwise only one batch is created.
      */
     void set_multiple_batches(bool value);

     /**
      * @returns True if multiple batches is used.
      */
     bool multiple_batches() const;

     /**
      * @param  value If true the batch can be processed in multiple nodes.
      */
     void set_parallel_processing(bool value);

     /**
      * @returns True if parallel processing is used.
      */
     bool parallel_processing() const;

     /**
      * @param  value If true it is allowed to start incomplete batches, i.e.
      * before all parts are available.
      */
     void set_start_incomplete(bool value);

     /**
      * @returns True if start incomplete is used.
      */
     bool start_incomplete() const;

     /**
      * @param  value If true creation of a new batch will prioritize complete
      * (i.e. available in sufficient quantities) batches.
      */
     void set_prioritize_complete(bool value);

     /**
      * @returns True if prioritize complete.
      */
     bool prioritize_complete() const;

     /**
      * @param  timeout see member variable incomplete_timeout_.
      */
     void set_incomplete_timeout(NumberGenerator* timeout);

     /**
      * @returns NumberGenerator for the incomplete timeout or nullptr,
      * see member variable incomplete_timeout_.
      */
     NumberGenerator* incomplete_timeout() const;

     /**
      * @brief Add a batch.
      *
      * @param variant The variant that should be associated with this batch.
      * @param size The number of entities in this batch.
      */
     void add_batch(Variant *variant, Int size);

     /**
      * @returns All batch items.
      */
     const std::vector<BatchItem>& batches() const;

     /**
      * @brief Add a demand.
      *
      * @param store The store that controls the demand.
      */
     void add_demand(Store *store);

     /**
      * @returns All demands.
      */
     const std::vector<Store*>& demands() const;

     /**
      * @brief Do the necessary actions when a entity enters the batch.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity that entered a node.
      */
     void entering(Node *departure, Node* destination, Entity *entity);

     /**
      * @brief Do the necessary actions when a entity exits the batch.
      *
      * @param departure The node the entity exited.
      * @param destination The node the entity entered.
      * @param entity The entity that exited.
      */
     void exiting(Node *departure, Node* destination, Entity *entity);

     /**
      * @brief Do the necessary actions when a entity leaves a node that
      * controls the demand.
      *
      * @param departure The node the entity exited.
      * @param destination The node the entity entered.
      * @param entity The entity that left.
      */
     void exited_demand(Node* departure, Node* destination, Entity* entity);

     /**
      * @brief Go through the block list and try to start a new batch at each
      * node.
      */
     void start_new_batch();

     /**
      * @brief Remove a entity from the work in process list.
      *
      * @param  departure   The node the entity is coming from.
      * @param  destination The node the entity going to.
      * @param  entity      The entity to remove.
      */
     void decrease_batch_wip(Node* departure, Node* destination, Entity *entity);

     /**
      * @brief Gets the number of batches that have been created for a
      * particular variant.
      *
      * @param variant The variant to get the batch count for.
      *
      * @returns The batch count.
      */
     int batch_count(Variant *variant) const;

     /**
      * @brief Gets the average batch size for a particular variant.
      *
      * @param variant The variant to get the average batch size for.
      *
      * @returns The average batch size.
      */
     double average_batch_size(Variant *variant) const;

      /**
       * @brief Locate the batch with id 'batch_id' and finish it even if it is
       * not completed.
       * 
       * Nothing will happen if:
       * 1. A matching batch isn't found.  
       * 2. A matching batch is found and active, i.e. all assigned entities  
       *    have not been completed.
       *
       * @param batch_id    Id of the batch that should be finished.
       */
      void finish_batch(unsigned int batch_id);

 private:
     typedef std::map<unsigned int, NodeItem> Batches;

     /**
      * @brief Try to start a new batch at a node, if demand is used it checks
      * that that there is sufficient demand.
      *
      * @param node The node where a batch should be created.
      *
      * @return True if a batch was created.
      */
     bool start_new_batch(Node *node);

     /**
      * @brief Create a new batch of a particular variant if there are enough
      * entities on the block list. No more batches are created than there
      * are demand for.
      *
      * @param variant Only entities of this variant can be part of this
      * batch.
      * @param entities All entities that are available.
      * @param demand The demand for the variant.
      *
      * @return True if batch was created.
      */
     bool create_new_batch(Variant *variant,
             const std::vector<BlockListIterator> &entities, int demand,
             bool start_incomplete);

     /**
      * @brief Check if a batch of a particular variant can be created.
      *
      * @param variant The variant that should be checked.
      *
      * @return True if it can be created.
      */
     bool is_handled(Variant *variant) const;

     /**
      * @brief Check if demand is used.
      *
      * @return True if it is used.
      */
     bool use_demand() const;

     /**
      * @brief How many entities that are in the batches.
      */
     std::map<Variant*, Int> batch_sizes_;

     /**
      * @brief All batches that can be created.
      */
     Batches batches_;

     /**
      * @brief How many batches that have been created.
      */
     std::map<Variant*, std::pair<unsigned int, unsigned int> > batch_count_;

     /**
      * @brief The number of entities of each variant that are currently
      * in this batch.
      */
     std::map<Variant*, std::list<Entity*> > batch_wip_;

     /**
      * @brief The order the batches were created. Used to print the output in
      * that same order.
      */
     std::vector<BatchItem> batch_order_;

     /**
      * @brief True if multiple batches is used.
      */
     bool multiple_batches_;

     /**
      * @brief True if parallel processing is used.
      */
     bool parallel_processing_;

     /**
      * @brief All demands.
      */
     std::vector<Store*> demands_;

     /**
      * @brief The start new event batch.
      */
     EventStartNewBatch *start_new_batch_event_;

     typedef std::map<Variant*, int> vc_map;
     typedef std::map<Node*, vc_map> dvc_map;
     /**
      * @brief Nested dictionaries that keeps track of number of entities per
      * variant available for batch creation at each destination node.
      *
      * First key is the destination node and second key is the variant.
      */
      dvc_map destination_variant_count_;

      /**
       * @brief Decrease variant count for all destination nodes.
       *
       * @param variant The variant to decrease count for.
       */
      void decrease_variant_count(Variant *variant);

      /**
       * @brief Checks whether or not there are enough entities of any
       * variant at the specified node to create an entire batch.
       *
       * If it is allowed to start incomplete batches this is always
       * True.
       *
       * @param node The node for which a batch should be considered.
       *
       * @return True if there are enought entities of at least one
       * variant to create a batch for the specified node, otherwise
       * false.
       */
      bool is_batch_possible(Node *node);

      /**
       * @brief Checks whether or not there are enough entities of the
       * specified variant at the specified node to create an entire batch.
       *
       * If it is allowed to start incomplete batches this is always
       * True.
       *
       * @param node The node for which a batch should be considered.
       * @param variant The variant for which a batch should be considered.
       *
       * @return True if there are enought entities of the specified variant
       * and node to create a batch, otherwise false.
       */
      bool is_batch_possible(Node *node, Entity *entity);

      /**
       * @brief True if it is allowed to start incomplete batches, i.e. when
       * all parts aren't available.
       */
      bool start_incomplete_;

      /**
       * @brief True if complete batches should be preferred over incomplete
       * ones when a new batch should be created.
       */
      bool prioritize_complete_;

      /**
       * @brief Timeout used to set the time that the batch logic will wait for
       * a matching entity to arrive to an incomplete batch. After the timeout
       * the incomplete batch will be finished even though it is not complete.
       *
       * If not set the batch logic will wait indefinitely for a matching entity
       * to arrive.
       */
      NumberGenerator* incomplete_timeout_;

      /**
       * @brief
       *
       * @param entity
       * @param batch_id
       */
      void assign_entity_to_batch(Entity *entity, unsigned int batch_id);
};

} // namespace xsim

#endif // BATCH_H
