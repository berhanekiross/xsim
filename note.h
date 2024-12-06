#ifndef NOTE_H
#define NOTE_H

#include <xsim_config>

#include "object.h"

namespace xsim {

/** @brief Notes. */
class XSIM_EXPORT Note : public Object {
 public:
     /** @brief Default constructor */
     Note();

     /**
      * @param  text The text to set.
      */
     void set_text(const std::string& text);

     /**
      * @returns The text.
      */
     const std::string& text() const;

private:

    /** @brief The text */
    std::string text_;

};

} // namespace xsim

#endif // NOTE_H
