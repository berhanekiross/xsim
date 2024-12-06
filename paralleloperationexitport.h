#ifndef PARALLELOPERATIONDEPARTURE_H
#define PARALLELOPERATIONDEPARTURE_H

#include <xsim_config>

#include "paralleloperation.h"
#include "movestrategy.h"
#include "exitport.h"

namespace xsim {

/**
 * @brief ParallelOperation specific departure.
 */
class XSIM_EXPORT ParallelOperationDeparture : public ExitPort {
 public:
     /**
      * @brief Constructor.
     */
     ParallelOperationDeparture(Node *node);

     void add_exit_logic(ExitLogic *logic) override;
};

} // namespace xsim

#endif // PARALLELOPERATIONDEPARTURE_H
