#ifndef LOGBUFFER_H
#define LOGBUFFER_H

#include <xsim_config>
#include <string>
#include <fstream>

namespace xsim {

class LogBuffer : public std::streambuf {
public:
    int_type overflow(int_type c) override;
    std::streamsize xsputn(const char* s, std::streamsize n) override;
    int sync() override;

private:
    std::string str_;
};

}

#endif
