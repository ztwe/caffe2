#ifndef CAFFE2_OPERATORS_WHILE_OP_H_
#define CAFFE2_OPERATORS_WHILE_OP_H_

#include "caffe2/core/context.h"
#include "caffe2/core/logging.h"
#include "caffe2/core/operator.h"

namespace caffe2 {

template <class Context>
class WhileOp final : public Operator<Context> {
 public:
  WhileOp(const OperatorDef& operator_def, Workspace* ws)
      : Operator<Context>(operator_def, ws) {
    CAFFE_ENFORCE(
        this->template HasSingleArgumentOfType<NetDef>("loop_net"),
        "loop_net must be specified in While operator");
    loop_net_def_ =
        this->template GetSingleArgument<NetDef>("loop_net", NetDef());
    loop_net_ = ws->CreateNet(loop_net_def_, true);
    CAFFE_ENFORCE(loop_net_, "Failed to initialize loop subnet");

    cond_net_ = nullptr;
    bool has_cond_net =
        this->template HasSingleArgumentOfType<NetDef>("cond_net");
    if (has_cond_net) {
      cond_net_def_ =
          this->template GetSingleArgument<NetDef>("cond_net", NetDef());
      cond_net_ = ws->CreateNet(cond_net_def_, true);
      CAFFE_ENFORCE(cond_net_, "Failed to initialize condition subnet");
    }
  }

  USE_OPERATOR_CONTEXT_FUNCTIONS;
  bool RunOnDevice() override;

 private:
  NetDef loop_net_def_;
  NetBase* loop_net_;

  NetDef cond_net_def_;
  NetBase* cond_net_;
};

} // namespace caffe2

#endif // CAFFE2_OPERATORS_WHILE_OP_H_
