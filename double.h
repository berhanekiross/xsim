#ifndef DOUBLE_H
#define DOUBLE_H

#include <xsim_config>
#include <list>
#include <string>

namespace mu {
class Parser;
}

namespace xsim {

class Simulation;

class XSIM_EXPORT Double {
public:
    Double();
    Double(double value);
    Double(const std::string& value);
    Double(const Double& d);
    ~Double();
    Double& operator=(const Double& rhs);
    operator double() const;

    double value() const;
    std::string to_string() const;
    mu::Parser* parser() const;

private:
    mu::Parser* parser_;
    double value_;
    std::string value_string_;
};

} // namespace xsim

#endif // DOUBLE_H
