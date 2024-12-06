#ifndef ASSEMBLYSPECIFICATION_H
#define ASSEMBLYSPECIFICATION_H

#include <xsim_config>
#include <map>
#include <utility>
#include <vector>

#include "int.h"

namespace xsim {

class Variant;
class Entity;
class Simulation;

     /**
      * @brief Helper struct for keeping track of all entities.
      *
      * @param num_parts The required amount of this variant type.
      * @param delete_on_assemble If true this variant should be
      * deleted upon assembly.
      * @param use_units If true this variant type is consumed in units.
      */
     struct EntitySpecification {
         EntitySpecification(
             Variant* variant,
             Int num_parts,
             bool delete_on_assemble,
             bool use_units) :
             variant_(variant),
             units_(0),
             use_units_(use_units),
             num_parts_(num_parts),
             delete_on_assemble_(delete_on_assemble) {}

         Variant* variant_;
         // Counter for number of recieved units.
         int units_;
         bool use_units_;
         Int num_parts_;
         bool delete_on_assemble_;
         // Vector containing all recieved entities.
         std::vector<Entity*> entities;
     };


/**
 * @brief Holds information how to assemble a particular type of assembly.
 *
 * Some terminology, the container is the top level entity, where all
 * other entities are assembled onto. The entities that are assembled
 * onto the container are called parts.
 */
class XSIM_EXPORT AssemblySpecification {
 public:
     /**
      * @brief Constructor.
      *
      * @param sim The simulation manager.
      */
     AssemblySpecification(Simulation *sim);

     /**
      * @brief Destructor.
      *
      * @param 
      */
     ~AssemblySpecification();

     /**
      * @brief Check if the container and all parts and have arrived.
      *
      * @return True if the container and all parts have arrived.
      */
     bool is_complete();

     /**
      * @brief Set the container variant.
      *
      * @param variant The variant.
      */
     void set_container_variant(Variant *variant);

     /**
      * @brief Get the entity that is set as the container.
      *
      * @return The container.
      */
     Entity* container() const { return container_; }

     /**
      * @brief Add how many of a specific variant that is needed by this
      * assembly specification to complete one assembly.
      *
      * The variants should only be added at the start of the simulation, they
      * only describe what variants that are needed for this assembly
      * specification.
      *
      * @param variant The variant to add.
      * @param value How many of this variant that is needed.
      * @param delete_on_assemble True if the entity should be deleted on
      * assemble.
      * @param use_units True if this variant should use units.
      */
     void add_variant(Variant *variant, Int value, bool delete_on_assemble,
            bool use_units);

     /**
      * @brief Gets entity specifications.
      *
      * @returns All entity specifications.
      */
     const std::vector<EntitySpecification*>& entity_specifications() const;

     /**
      * @brief Add a untyped variant that matches all variants. The value
      * specify how many variants that should be matches.
      *
      * @param value How many entities of any variant that are needed. If zero
      * it means that area is used to determine how many to assemble.
      * @param delete_on_assemble True if the entity should be deleted on
      * assemble.
      * @param use_units True if this variant should use units.
      */
     void add_untyped_variant(int value, bool delete_on_assemble,
            bool use_units);

     /**
      * @returns All untyped entities.
      */
     EntitySpecification* untyped_entities() const;

     /**
      * @brief Get the container variant.
      *
      * @return The container variant.
      */
     Variant* container_variant() const;

     /**
      * @brief Set the variant that should be the assembly identity.
      *
      * @param variant The variant to use for the assembly identity.
      */
     void set_assembly_identity(Variant *variant);

     /**
      * @brief Get the variant that is the assembly identity.
      *
      * @return The assembly identity variant.
      */
     Variant* assembly_identity() const;

     /**
      * @brief Assemble the parts onto the container. Adds wip time to any
      * deleted entities.
      * @param elapsed The elapsed simulation time.
      * @param warmup The warmup time.
      */
     void assemble(double elapsed, double warmup);

     /**
      * @brief Check if the assembly specification is complete and assembled.
      *
      * @return True if it is assembled.
      */
     bool is_assembled() const;

     /**
      * @brief Reset the container and all parts, so that a new assembly can be
      * started.
      */
     void clear();

     /**
      * @brief Check if a particular variant is needed by this assembly
      * specification.
      *
      * @param variant The variant that is checked.
      *
      * @return How many of this variant that is needed.
      */
     int need_variant(Variant *variant) const;

     /**
      * @brief Add a newly arrived entity.
      *
      * This is used to keep track of how many entities of each variant
      * that is currently waiting for assembly.
      *
      * @param entity The entity to add.
      */
     void add_entity(Entity *entity);

     /**
      * @brief Add units of a specific variant type.
      *
      * @param variant The variant type that is receiving units.
      * @param units The amount of units to add.
      */
     void add_units(Variant *variant, int units);

     /**
      * @brief Checks if any active specifications use units.
      *
      * This function assumes that a variant is not present in two different
      * assembly specifications where it uses units in one and not in the
      * other.
      *
      * @param variant The variant that should be checked.
      * @param consider_wildcard If true a wildcard variant is
      * considered a match.
      *
      * @return True if the variant use units.
      */
     bool use_units(Variant *variant, bool consider_wildcard) const;

     /**
      * @brief Check if a variant is present in this specification.
      *
      * @param variant The variant that should be checked.
      * @param consider_wildcard If true a wildcard variant is
      * considered a match.
      *
      * @return True if the variant is present.
      */
     bool has_variant(Variant *variant, bool consider_wildcard) const;

     /**
      * @brief Get the total of the container and the maximum number of parts
      * this assembly specification can have.
      *
      * @return The maximum size of this assembly specification.
      */
     unsigned int max_size() const;

     /**
      * @brief Get the size of all parts plus the container.
      *
      * @return 
      */
     size_t size() const;

     /**
      * @brief States if a untyped variant is present or not.
      *
      * @return If true this specification contains an
      * untyped variant.
      */
     bool has_untyped_variant() const;

     /**
      * @brief If true a untyped variant is inclued in
      * this specification with use_units = true.
      * 
      * @return True if use_units is true for any
      * untyped variant of this specification.
      */
     bool untyped_use_units() const;
         
     /**
      * @brief Getter for the entity specification associated
      * with a certain variant type.
      *
      * @param variant The variant type mathcing the desired
      * entity specification.
      *
      * @return Entity specification, if found.
      */
     EntitySpecification* get_entity_specification(Variant *variant) const;

 private:

     typedef std::map<Variant*, EntitySpecification*> Parts;

     /**
      * @brief Set the container to a entity.
      *
      * @param entity The entity that should be set.
      */
     void set_container(Entity *entity);

     /**
      * @brief Add entity as a part.
      *
      * @param entity The entity to add.
      */
     void add_part(Entity *entity);

     /**
      * @brief Check if a particular variant is needed by the assembly.
      *
      * @param variant The variant that should be checked.
      *
      * @return variant How many of this part that is needed.
      */
     int need_part(Variant *variant) const;

     /**
      * @brief Add untyped entity as a part.
      *
      * @param entity The entity to add.
      */
     void add_untyped_part(Entity *entity);

     /**
      * @brief The simulation manager.
      */
     Simulation *sim_;

     /**
      * @brief The type of variant that should be used as the container.
      */
     Variant *container_variant_;

     /**
      * @brief The variant that should be used for routing and determining
      * setup and processing times.
      */
     Variant *assembly_identity_;

     /**
      * @brief The entity instance that is considered to be the container.
      */
     Entity *container_;

     /**
      * @brief If true the assembly specification is complete and and assembled.
      */
     bool assembled_;

     /**
      * @brief How many of each variant that is needed and how many is
      * currently present.
      */
     Parts parts_;

     double area_used_;

     /**
      * @brief How many of untyped entities that is accepted by the specification.
      */
     EntitySpecification* untyped_entities_;

     /**
      * @brief We need to preserve the order in which the variants was added,
      * because 'parts_' currently uses a map which don't preserve the order.
      * So we use this one instead.
      */
     std::vector<EntitySpecification*> variant_order_;
};

} // namespace xsim

#endif // ASSEMBLYSPECIFICATION_H
