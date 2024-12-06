#ifndef OUTPUT_H
#define OUTPUT_H

#include <xsim_config>
#include <string>
#include <vector>
#include <functional>

namespace xsim {

class XSIM_EXPORT Output {
public:

    /**
     * @brief Constructor
     *
     * @param  name The name of the output.
     * @param  type The type.
     * @param  func The function used to set the output value.
     */
    Output(const std::string &name, const std::string &type, std::function<double()> func);

    /**
     * @brief Constructor
     *
     * @param  name The name of the output.
     * @param  type The type of the output.
     */
    Output(const std::string &name, const std::string &type);

    /** @brief Destructor */
    ~Output();

    /**
     * @brief Define a output under this output.
     *
     * @param  name The name of the output.
     * @param  type The type of the output.
     * @param  func The function used to set the output value.
     */
    void define_output(
            const std::string &name,
            const std::string &type,
            std::function<double()> func);

    /**
     * @brief Gets the outputs that are grouped under this output.
     *
     * @returns The outputs.
     */
    const std::vector<Output*> outputs() const;

    /**
     * @brief Gets the name
     *
     * @returns The name of the output.
     */
    const std::string& name() const;

    /**
     * @brief Gets the type.
     *
     * @returns The type.
     */
    const std::string& type() const;

    /**
     * @brief Returns true if this output does not have a function set.
     *
     * If there is no function set then this is an empty output that won't
     * produce any output values.
     *
     * @returns True if this output is empty.
     */
    bool empty() const;

    /**
     * @brief Gets all replication values
     *
     * @returns All replication values
     */
    const std::vector<double>& values() const;

    /** @brief Sets a replication value using the the output function */
    void set();

    /** @brief Clears this object to its blank/initial state */
    void clear();

    /**
     * @brief Determines the average value of all replications.
     *
     * @returns The average value.
     */
    double average() const;

private:
    /** @brief The name of the output */
    std::string name_;

    /** @brief The type of the output */
    std::string type_;

    /** @brief The output function that generates the value */
    std::function<double()> func_;

    /** @brief The output values from all replications */
    std::vector<double> values_;

    /** @brief The outputs that are grouped under this output */
    std::vector<Output*> outputs_;
};

} // namespace xsim

#endif // OUTPUT_H