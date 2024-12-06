#ifndef PARALLELOPERATIONEXITLOGIC_H
#define PARALLELOPERATIONEXITLOGIC_H

#include <xsim_config>
#include <string>

#include "exitlogic.h"
#include "simulation.h"

namespace xsim {

class ParallelOperation;
class Node;

/**
 * @brief Moves entities in sync between nodes.
 */
class XSIM_EXPORT ParallelOperationExitLogic : public ExitLogic {
 public:
    ParallelOperationExitLogic(ParallelOperation *parallel_operation);

    bool allow_leave(Node *node, Entity *entity) override;

 private:
    ParallelOperation *parallel_operation_;
};

} // namespace xsim

#endif // PARALLELOPERATIONEXITLOGIC_H
