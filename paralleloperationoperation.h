#ifndef PARALLELOPERATIONOPERATION_H
#define PARALLELOPERATIONOPERATION_H

#include <xsim_config>

#include "operation.h"

namespace xsim {

class ParallelOperation;

/**
 * @brief ParallelOperation specific operation.
 */
class XSIM_EXPORT ParallelOperationOperation : public Operation {
public:
    ParallelOperationOperation(ParallelOperation* parallel_operation, int id);

    bool enter(Entity* entity, Node* departure) override;
    void leave(Entity* entity, Node* destination) override;

    /**
     * @returns True if this operation has finished processing an entity, otherwise false.
     */
    bool finished() const;

    /**
     * @brief Get the parallel operation operation id.
     *
     * This id is guaranteed to be unique only for parallel operation operations within a given
     * parallel operation. It is also the index that corresponds to this operation in the owning
     * parallel operation.
     *
     * @return A zero based index.
     */
    virtual int parallel_operation_id() const;

private:
    int id_;

    /** @brief True if it has finished processing */
    bool finished_ = false;

    /**
     * @brief The owning parallel operation.
     */
    ParallelOperation* parallel_operation_;
};

} // namespace xsim

#endif // PARALLELOPERATIONOPERATION_H
