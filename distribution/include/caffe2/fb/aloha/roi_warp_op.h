// Copyright 2004-present Facebook. All Rights Reserved.

#ifndef ROI_WARP_OP_H_
#define ROI_WARP_OP_H_

#include "caffe2/core/context.h"
#include "caffe2/core/logging.h"
#include "caffe2/core/operator.h"
#include "caffe2/utils/math.h"

namespace caffe2 {

template <typename T, class Context>
class RoIWarpOp final : public Operator<Context> {
 public:
  RoIWarpOp(const OperatorDef& operator_def, Workspace* ws)
      : Operator<Context>(operator_def, ws),
        spatial_scale_(OperatorBase::GetSingleArgument<float>(
              "spatial_scale", 1.)),
        pooled_height_(OperatorBase::GetSingleArgument<int>("pooled_h", 1)),
        pooled_width_(OperatorBase::GetSingleArgument<int>("pooled_w", 1)),
        sampling_ratio_(OperatorBase::GetSingleArgument<int>("sampling_ratio", -1)) {
    DCHECK_GT(spatial_scale_, 0);
    DCHECK_GT(pooled_height_, 0);
    DCHECK_GT(pooled_width_, 0);
    DCHECK_GE(sampling_ratio_, 0);
  }
  USE_OPERATOR_CONTEXT_FUNCTIONS;

  bool RunOnDevice() override {
    CAFFE_NOT_IMPLEMENTED;
    return true;
  }

 protected:
  float spatial_scale_;
  int pooled_height_;
  int pooled_width_;
  int sampling_ratio_;
};

template <typename T, class Context>
class RoIWarpGradientOp final : public Operator<Context> {
 public:
  RoIWarpGradientOp(const OperatorDef& def, Workspace* ws)
      : Operator<Context>(def, ws),
        spatial_scale_(OperatorBase::GetSingleArgument<float>(
              "spatial_scale", 1.)),
        pooled_height_(OperatorBase::GetSingleArgument<int>("pooled_h", 1)),
        pooled_width_(OperatorBase::GetSingleArgument<int>("pooled_w", 1)),
        sampling_ratio_(OperatorBase::GetSingleArgument<int>("sampling_ratio", -1)) {
    DCHECK_GT(spatial_scale_, 0);
    DCHECK_GT(pooled_height_, 0);
    DCHECK_GT(pooled_width_, 0);
    DCHECK_GE(sampling_ratio_, 0);
  }
  USE_OPERATOR_CONTEXT_FUNCTIONS;

  bool RunOnDevice() override {
    CAFFE_NOT_IMPLEMENTED;
    return true;
  }

 protected:
  float spatial_scale_;
  int pooled_height_;
  int pooled_width_;
  int sampling_ratio_;
};

} // namespace caffe2

#endif // ROI_POOL_OP_H_
