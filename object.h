#ifndef XOBJECT_H
#define XOBJECT_H

#include <xsim_config>
#include <functional>
#include <string>
#include <vector>

#include "signal.hpp"

namespace xsim {

class Output;
class Component;

struct UserAttribute {
    std::string id;
    std::string name;
    std::string data_type;
    std::string value;
};
class PropertyContainer;

/** @brief Base class for objects. */
class XSIM_EXPORT Object {
 public:
     Signal<void (Object*)> simulation_initialized;
     Signal<void (Object*)> simulation_finalized;
     Signal<void (Object*)> initialized;
     Signal<void (Object*)> finalized;
     Signal<void (Object*)> statistics_reset;

     /** @brief Constructor */
     Object();

     /** Copying and moving objects is not supported. */
     Object(const Object&) = delete;
     Object(Object&&) = delete;
     Object& operator=(const Object&) = delete;
     Object& operator=(Object&&) = delete;

     /** @brief Destructor */
     virtual ~Object();

     /** @brief Initialize the object at the start of the simulation. */
     virtual void simulation_init();

     /** @brief Finalize the object at the end of the simulation. */
     virtual void simulation_finalize();

     /** @brief Initialize the object at the start of a simulation replication. */
     virtual void init();

     /** @brief Finalize the object at the end of a simulation replication. */
     virtual void finalize();

     /** @brief Reset statistics that have been collected so far. */
     virtual void reset_stats();

     /**
      * @brief Adds a child to this object
      *
      * @param child The child to add.
      */
     void add_child(Object* child);

     /**
      * @brief Removes a child from this object.
      *
      * @param child The child to remove.
      */
     void remove_child(Object* child);

     /**
      * @brief Replace a child.
      *
      * @param old_child The child to replace, if it is not found the new child will be added.
      * @param new_child The new child, if nullptr then old child will be removed and not replaced.
      */
     void replace_child(Object* old_child, Object* new_child);

     /**
      * @brief Get the child at the specified index.
      *
      * @param  index The zero-based index of the child.
      *
      * @returns The child if the index is valid, otherwise nullptr.
      */
     Object* child(int index) const;

     /**
      * @brief Gets the number children of this object
      *
      * @returns The total number of children.
      */
     size_t children_size() const;

     /**
      * @brief Gets the index of a child.
      *
      * @param  object The child to find the index for.
      *
      * @returns The index of the child if found, otherwise -1.
      */
     int child_index(Object* object) const;

     /**
      * @brief Enable or disable this object.
      *
      * @param  value True if the object should be enabled, otherwise false.
      */
     void set_enabled(bool value);

     /**
      * @brief Gets if this object is enabled.
      *
      * @param  recursive If true then all parents (and their parents) must also be enabled for this
      *                   object to be considered enabled. If false the check is only made for this
      *                   object.
      *
      * @returns True if the object is enable, otherwise false.
      */
     bool enabled(bool recursive = true) const;

     /**
      * @param parent The parent to add.
      */
     void add_parent(Object* parent);

     /**
      * @param parent The parent to remove.
      */
     void remove_parent(Object* parent);

     /**
      * @returns The parents of this object.
      */
     const std::vector<Object*>& parents() const;

     /**
      * @returns The number of parents.
      */
     size_t parents_size() const;

     /**
      * @brief Get the the type of this object.
      *
      * @return The type as a string.
      */
     virtual std::string type() const;

     /**
      * @brief Set the type of this object.
      *
      * @param type The new type.
      */
     virtual void set_type(const std::string &type);

     /**
      * @brief Get the name of this object.
      *
      * @return The name.
      */
     virtual std::string name() const;

     /**
      * @brief Set the name of this object.
      *
      * @param name The new name.
      */
     virtual void set_name(const std::string &name);

     /**
      * @brief Get the path of this object.
      *
      * If the object is located at the root this is the same as the name,
      * otherwise it is the recursively combined name of all parents of this
      * object plus its own name.
      *
      * @return The path.
      */
     virtual std::string path() const;

     /**
      * @brief Set the path of this object.
      *
      * @param path The new path.
      */
     virtual void set_path(const std::string &path);

     /**
      * @param  value The X position to set.
      */
     void set_xpos(float value);

     /**
      * @returns The X position.
      */
     float xpos() const;

     /**
      * @param  value The Y position to set.
      */
     void set_ypos(float value);

     /**
      * @returns The Y position.
      */
     float ypos() const;

     /**
      * @brief Get the unique identifier of this object.
      *
      * @return The id.
      */
     virtual std::string id() const;

     /**
      * @brief Set the id of this object.
      *
      * @param id The new id.
      */
     virtual void set_id(const std::string &id);


     /**
      * @brief Define a new output.
      *
      * Examples on how to use this function.:
      *
      * // Example non member function
      * double th()
      * {
      *     return 10.0;
      * }
      *
      * // Example member function
      * double ExampleClass::th()
      * {
      *     return 10.0;
      * }
      *
      * Using a lambda function on an object with the member function th()
      *   define_output("throughput", [=] { return th(); })
      *
      * Using a lambda function with a non-member function th()
      *   define_output("throughput", [] { return th(); })
      *
      * Using std::bind on an object of type ExampleClass with the function th()
      *   define_output("throughput", std::bind(&ExampleClass::th, this))
      *
      * Using std::bind on an object of type ExampleClass with the function
      * th(int a) with the parameter value 5.
      *   define_output("throughput", std::bind(&ExampleClass::th, this, 5))
      *
      * Using an regular (non-member) function th()
      *   define_output("throughput", th);
      *
      * @param name The name of the output.
      * @param func The function that will set the value of the output.
      *
      * @returns The output
      */
     Output* define_output(const std::string &name, std::function<double()> func);

     /**
      * @brief Define an empty output.
      *
      * An empty output is not useful in itself, but it can be used for
      * grouping together other outputs.
      *
      * @param name The name of the output.
      *
      * @returns The output
      */
     Output* define_output(const std::string &name);

     /**
      * @brief Get all outputs.
      *
      * @returns All outputs
      */
     const std::vector<Output*>& outputs() const;

     /**
      * @brief Gets a specific output
      *
      * @param [in] name The name of the output.
      *
      * @returns The output.
      */
     Output* get_output(const std::string &name);

     /**
      * @brief Define stats associated with this object.
      */
     virtual void define_outputs();

     /**
      * @brief Set stats associated with this object.
      */
     virtual void set_outputs();

     /**
      * @brief Query if this object is simulation initialized.
      *
      * @returns True if initialized, false if not.
      */
     bool is_simulation_initialized() const;

     /**
      * @brief Query if this object is simulation finalized.
      *
      * @returns True if finalized, false if not.
      */
     bool is_simulation_finalized() const;

     /**
      * @brief Get the property container.
      *
      * @return The property container.
      */
     PropertyContainer* const properties() const;

     /**
      * @brief Query if this object is initialized.
      *
      * @returns True if initialized, false if not.
      */
     bool is_initialized() const;

     /**
      * @brief Query if this object is finalized.
      *
      * @returns True if finalized, false if not.
      */
     bool is_finalized() const;

     /**
      * @brief Query if this object has set outputs.
      *
      * @returns True if outputs has been set, false if not.
      */
     bool is_outputs_set() const;

     /**
      * @brief Gets the closest parent that is a Component.
      *
      * @returns The parent component.
      */
     Component* get_parent_component();

     /**
      * @brief Adds a user attribute
      *
      * @param  id        The identifier.
      * @param  name      The name.
      * @param  data_type Type of the data.
      * @param  value     The value.
      */
     void add_user_attribute(const std::string& id,
                             const std::string& name,
                             const std::string& data_type,
                             const std::string& value);

     /**
      * @returns All user attributes.
      */
     const std::vector<UserAttribute>& user_attributes() const;

     /**
      * @brief Gets all objects that is of a specified class.
      *
      * Note that this will only return objects that are of the exact same
      * class.
      *
      * @tparam T The class type that should be returned.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns All objects of the specified type.
      */
     template<typename T>
     std::vector<T*> class_objects(bool recursive = true) const
     {
         std::vector<T*> v;
         for (Object* object : children_) {
             if (typeid(*object).hash_code() == typeid(T).hash_code()) {
                 T* t = dynamic_cast<T*>(object);
                 if (t)
                     v.push_back(t);
             }
         }

         if (recursive) {
             for (Object* object : children_) {
                 std::vector<T*> cv = object->class_objects<T>(recursive);
                 v.insert(v.end(), cv.begin(), cv.end());
             }
         }

         return v;
     }

     /**
      * @brief Gets all objects that can be cast to a specified class.
      *
      * Note that this will return all objects that can be cast to T. So a call
      * to "class_objects<Node>()" will return all objects that are of type Node.
      *
      * @tparam T The class type that should be returned.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns All objects of the specified type.
      */
     template<typename T>
     std::vector<T*> polymorphic_objects(bool recursive = true) const
     {
         std::vector<T*> v;
         for (Object* object : children_) {
             T* t = dynamic_cast<T*>(object);
             if (t)
                 v.push_back(t);

         }

         if (recursive) {
             for (Object* object : children_) {
                 std::vector<T*> cv = object->polymorphic_objects<T>(recursive);
                 v.insert(v.end(), cv.begin(), cv.end());
             }
         }

         return v;
     }

     /**
      * @brief Gets all objects of a specific object type.
      *        
      * This function only returns objects that have the exact same object type.
      *
      * @param  type      The object type that should be returned.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns All objects that matches the specified object type.
      */
     std::vector<Object*> type_objects( const std::string& type, bool recursive = true) const;

     /**
      * @brief Get node from id.
      *
      * @tparam T Generic type parameter.
      * @param  id        The id of the object.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns The object if found, otherwise a nullptr.
      */
     template<typename T = Object>
     T* find_object_by_id(const std::string& id, bool recursive = true) const
     {
         for (Object* object : children_) {
             if (object->id() == id)
                 return dynamic_cast<T*>(object);

             if (recursive) {
                 T* t = object->find_object_by_id<T>(id, recursive);
                 if (t)
                     return t;
             }
         }

         return nullptr;
     }

     /**
      * @brief Get node from name.
      *
      * @tparam T Generic type parameter.
      * @param  name      The name of the object.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns The object if found, otherwise a nullptr.
      */
     template<typename T = Object>
     T* find_object_by_name(const std::string& name, bool recursive = true) const
     {
         for (Object* object : children_) {
             if (object->name() == name)
                 return dynamic_cast<T*>(object);

             if (recursive) {
                 T* t = object->find_object_by_name<T>(name, recursive);
                 if (t)
                     return t;
             }
         }

         return nullptr;
     }

     /**
      * @brief Searches for the first object of specific class
      *
      * @tparam T The object type that should be searched for.
      * @param  recursive True to process recursively, false to process this object only.
      *
      * @returns The object if found, otherwise a nullptr.
      */
     template<typename T>
     T* find_object(bool recursive = true) const
     {
         for (Object* object : children_) {
             T* t = dynamic_cast<T*>(object);
             if (t)
                 return t;

             if (recursive) {
                 T* t = object->find_object<T>(recursive);
                 if (t)
                     return t;
             }
         }

         return nullptr;
     }

     /**
      * @brief Find the closest parent that is of type T.
      *
      * @tparam T The type of the object to find.
      * @param recursive True if the search should continue beyond the immediate parent.
      * @return An object of type T if the search was successfull, otherwise nullptr.
      */
     template<typename T>
     T* find_parent(bool recursive = true)
     {
         for (Object* parent : parents()) {
             if (T* t = dynamic_cast<T*>(parent))
                 return t;
         }

         if (recursive) {
             for (Object* parent : parents()) {
                 T* t = parent->find_parent<T>(recursive);
                 if (t)
                     return t;
             }
         }

         return nullptr;
     }


private:
     void validate_output_name(const std::string& name);

     /** @brief True if this object is enabled. */
     bool enabled_;

     /** @brief The parents of this object. */
     std::vector<Object*> parents_;

     /**
      * @brief The type of this object.
      */
     std::string type_;

     /**
      * @brief The name of this object.
      */
     std::string name_;

     /**
      * @brief A unique identifier.
      */
     std::string id_;

     /**
      * @brief The path of this object.
      */
     std::string path_;

     /** @brief The X position */
     float xpos_;

     /** @brief The Y position */
     float ypos_;

     /**
      * @brief Holds all output values.
      */
     std::vector<Output*> outputs_;

     /** @brief True once simulation initialization is complete */
     bool simulation_initialized_;

     /** @brief True once simulation finalization is complete */
     bool simulation_finalized_;

     /**
      * @brief A container for all properties associated with this node.
      */
     PropertyContainer *properties_;

     /** @brief True once initialization is complete */
     bool initialized_;

     /** @brief True once finalization is complete */
     bool finalized_;

     /** @brief True once outputs has been set */
     bool outputs_set_;

     /** @brief The children of this object */
     std::vector<Object*> children_;

     /** @brief The user attributes */
     std::vector<UserAttribute> attributes_;

     /** @brief A unique id. */
     static inline int object_id_ = 0;
};

} // namespace xsim

#endif // OBJECT_H
