// Copyright 2004-present Facebook. All Rights Reserved.

#ifndef BBOX_TRANSFORM_OP_H_
#define BBOX_TRANSFORM_OP_H_

#include "caffe2/core/context.h"
#include "caffe2/core/logging.h"
#include "caffe2/core/operator.h"
#include "caffe2/utils/math.h"

namespace caffe2 {

template <typename T, class Context>
class BBoxTransformOp final : public Operator<Context> {
 public:
  BBoxTransformOp(const OperatorDef& operator_def, Workspace* ws)
      : Operator<Context>(operator_def, ws) {}
  USE_OPERATOR_CONTEXT_FUNCTIONS;

  bool RunOnDevice() override;

};

} // namespace caffe2

#endif // BBOX_TRANSFORM_OP_H_
