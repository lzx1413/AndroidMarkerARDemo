// Copyright 2004-present Facebook. All Rights Reserved.

#ifndef BOX_WITH_NMS_AND_LIMIT_OP_H_
#define BOX_WITH_NMS_AND_LIMIT_OP_H_

#include "caffe2/core/context.h"
#include "caffe2/core/operator.h"

namespace caffe2 {

// C++ implementation of function insert_box_results_with_nms_and_limit()
template <class Context>
class BoxWithNMSLimitOp final : public Operator<Context> {
 public:
  USE_OPERATOR_CONTEXT_FUNCTIONS;
  BoxWithNMSLimitOp(const OperatorDef& operator_def, Workspace* ws)
      : Operator<Context>(operator_def, ws),
        score_thres_(
            OperatorBase::GetSingleArgument<float>("score_thresh", 0.05)),
        nms_thres_(
            OperatorBase::GetSingleArgument<float>("nms", 0.3)),
        detections_per_im_(
            OperatorBase::GetSingleArgument<int>("detections_per_im", 100)) {}

  ~BoxWithNMSLimitOp() {}

  bool RunOnDevice() override;

 protected:
  // TEST.SCORE_THRESH
  float score_thres_ = 0.05;
  // TEST.NMS
  float nms_thres_ = 0.3;
  // TEST.DETECTIONS_PER_IM
  int detections_per_im_ = 100;
};

} // namespace caffe2
#endif // BOX_WITH_NMS_AND_LIMIT_OP_H_
