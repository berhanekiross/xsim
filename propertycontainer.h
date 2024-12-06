#ifndef PROPERTYCONTAINER_H
#define PROPERTYCONTAINER_H

#include <xsim_config>
#include <any>
#include <string>
#include <sstream>
#include <map>
#include <variant>
#include <functional>

namespace xsim {

/**
 * @brief A PropertyContainer store properties as key / value pairs that can be manipulated at
 *        runtime.
 */
class XSIM_EXPORT PropertyContainer {
 public:
     typedef std::variant<std::string, int, void*> PropertyKey;

     /**
      * @brief Delete all properties.
      *
      */
    void clear();

     /**
      * @brief Copies all properties to another property container.
      *
      * @param[in] properties The property container to copy properties to.
      */
     void copy(PropertyContainer *properties) const;

     /**
      * @brief Remove a property on the object.
      *
      * @param[in] key The property key to remove.
      */
     template <typename V>
     void remove(const PropertyKey& key)
     {
         auto&& i = properties_.find(key);
         if (i != properties_.end())
             properties_.erase(i);
     }
     template<>
     void remove<double>(const PropertyKey& key)
     {
         auto&& i = double_properties_.find(key);
         if (i != double_properties_.end())
             double_properties_.erase(i);
     }
     template<>
     void remove<int>(const PropertyKey& key)
     {
         auto&& i = int_properties_.find(key);
         if (i != int_properties_.end())
             int_properties_.erase(i);
     }

     /**
      * @brief Returns true if a property has been set.
      *
      * @param[in] key The property key to look for.
      *
      * @return True if the property exists.
      */
     template <typename V>
     bool has(const PropertyKey& key)
     {
         return properties_.find(key) != properties_.end();
     }
     template<>
     bool has<double>(const PropertyKey& key)
     {
         return double_properties_.find(key) != double_properties_.end();
     }
     template<>
     bool has<int>(const PropertyKey& key)
     {
         return int_properties_.find(key) != int_properties_.end();
     }

     /**
      * @brief Set a property on the object.
      *
      * @param key The name of the property.
      * @param value A value to store on the object.
      */
     template <typename V>
     void set(const PropertyKey& key, const V& value)
     {
         properties_[key] = value;
     }
     template<>
     void set<double>(const PropertyKey& key, const double& value)
     {
         double_properties_[key] = value;
     }
     template<>
     void set<int>(const PropertyKey& key, const int& value)
     {
         int_properties_[key] = value;
     }

     /**
      * @brief Attempts to update a property using a supplied function.
      *
      * @param[in] key The name of the property to update.
      * @param  func The function used to update the property.
      *
      * @returns True if the property exists, false otherwise.
      */
     template <typename V>
     bool try_update(const PropertyKey& key, std::function<V (const V&)> func)
     {
         auto&& i = properties_.find(key);
         if (i == properties_.end())
             return false;
         i->second = func(std::any_cast<V>(i->second));
         return true;
     }
     template<>
     bool try_update<double>(const PropertyKey& key, std::function<double (const double&)> func)
     {
         auto&& i = double_properties_.find(key);
         if (i == double_properties_.end())
             return false;
         i->second = func(i->second);
         return true;
     }
     template<>
     bool try_update<int>(const PropertyKey& key, std::function<int (const int&)> func)
     {
         auto&& i = int_properties_.find(key);
         if (i == int_properties_.end())
             return false;
         i->second = func(i->second);
         return true;
     }

     /**
      * @brief Get a property on the object.
      *
      * @param[in] key The name of the property to retrieve.
      * @param[out] value The value stored on the the object.
      *
      * @return True if property exists.
      */
     template <typename V>
     bool try_get(const PropertyKey& key, V& value) const
     {
         auto&& i = properties_.find(key);
         if (i == properties_.end())
             return false;

         value = std::any_cast<V>(i->second);
         return true;
     }
     template<>
     bool try_get<double>(const PropertyKey& key, double& value) const
     {
         auto&& i = double_properties_.find(key);
         if (i == double_properties_.end())
             return false;

         value = i->second;
         return true;
     }
     template<>
     bool try_get<int>(const PropertyKey& key, int& value) const
     {
         auto&& i = int_properties_.find(key);
         if (i == int_properties_.end())
             return false;

         value = i->second;
         return true;
     }

     /**
      * @brief Get a property on the object.
      *
      * @param[in] key The name of the property to retrieve.
      *
      * @return The value stored on the the object.
      */
     template<typename V>
     V get(const PropertyKey& key) const
     {
         auto&& p = properties_.find(key);
         if (p == properties_.end()) {
             std::stringstream ss;
             std::string name = std::visit([](auto&& arg) -> std::string {
                 using C = std::decay_t<decltype(arg)>;
                 if constexpr (std::is_same_v<C, std::string>) {
                     return arg;
                 }
                 else if constexpr (std::is_same_v<C, int>) {
                     return std::to_string(arg);
                 }
                 else if constexpr (std::is_same_v<C, void*>) {
                     std::stringstream ss;
                     ss << arg;
                     return ss.str();
                 }
                 else {
                     return "";
                 }
             }, key);
             throw std::runtime_error("Property does not exist: " + name);
         }

         return std::any_cast<V>(p->second);
     }
     template<>
     double get<double>(const PropertyKey& key) const
     {
         auto&& p = double_properties_.find(key);
         if (p == double_properties_.end()) {
             std::stringstream ss;
             std::string name = std::visit([](auto&& arg) -> std::string {
                 using C = std::decay_t<decltype(arg)>;
                 if constexpr (std::is_same_v<C, std::string>) {
                     return arg;
                 }
                 else if constexpr (std::is_same_v<C, int>) {
                     return std::to_string(arg);
                 }
                 else if constexpr (std::is_same_v<C, void*>) {
                     std::stringstream ss;
                     ss << arg;
                     return ss.str();
                 }
                 else {
                     return "";
                 }
             }, key);
             throw std::runtime_error("Property does not exist: " + name);
         }

         return p->second;
     }
     template<>
     int get<int>(const PropertyKey& key) const
     {
         auto&& p = int_properties_.find(key);
         if (p == int_properties_.end()) {
             std::stringstream ss;
             std::string name = std::visit([](auto&& arg) -> std::string {
                 using C = std::decay_t<decltype(arg)>;
                 if constexpr (std::is_same_v<C, std::string>) {
                     return arg;
                 }
                 else if constexpr (std::is_same_v<C, int>) {
                     return std::to_string(arg);
                 }
                 else if constexpr (std::is_same_v<C, void*>) {
                     std::stringstream ss;
                     ss << arg;
                     return ss.str();
                 }
                 else {
                     return "";
                 }
             }, key);
             throw std::runtime_error("Property does not exist: " + name);
         }

         return p->second;
     }

private:
    std::map<PropertyKey, std::any> properties_;
    std::map<PropertyKey, double> double_properties_;
    std::map<PropertyKey, int> int_properties_;
};

} // namespace xsim

#endif // PROPERTYCONTAINER_H
