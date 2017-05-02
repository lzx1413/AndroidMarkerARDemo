// Copyright 2004-present Facebook. All Rights Reserved.

#ifndef CAFFE2_OPERATORS_UTILS_NMS_H_
#define CAFFE2_OPERATORS_UTILS_NMS_H_

#include "caffe2/core/logging.h"
#include "caffe2/fb/aloha/eigen_utils.h"
#include "caffe2/utils/math.h"

namespace caffe2 {
namespace utils {

// Greedy non-maximum suppression for proposed bounding boxes
// Reject a bounding box if its region has an intersection-overunion (IoU)
//    overlap with a higher scoring selected bounding box larger than a
//    threshold.
// Reference: detection.caffe2/lib/nms/py_cpu_nms.py
// proposals: pixel coordinates of proposed bounding boxes,
//    size: (M, 4), format: [x1; y1; x2; y2]
// scores: scores for each bounding box, size: (M, 1)
// sorted_indices: indices that sorts the scores from high to low
// return: row indices of the selected proposals
template <class Derived1, class Derived2>
std::vector<int> nms_cpu(
    const Eigen::ArrayBase<Derived1>& proposals,
    const Eigen::ArrayBase<Derived2>& scores,
    const std::vector<int>& sorted_indices,
    float thresh) {
  CAFFE_ENFORCE_EQ(proposals.rows(), scores.rows());
  CAFFE_ENFORCE_EQ(proposals.cols(), 4);
  CAFFE_ENFORCE_EQ(scores.cols(), 1);
  CAFFE_ENFORCE_LE(sorted_indices.size(), proposals.rows());

  using EArrX = EArrXt<typename Derived1::Scalar>;

  auto x1 = proposals.col(0);
  auto y1 = proposals.col(1);
  auto x2 = proposals.col(2);
  auto y2 = proposals.col(3);

  EArrX areas = (x2 - x1 + 1.0) * (y2 - y1 + 1.0);

  EArrXi order = AsEArrXt(sorted_indices);
  std::vector<int> keep;
  int ci = 0;
  while (order.size() > 0) {
    int i = order[0];
    keep.push_back(i);
    ConstEigenVectorArrayMap<int> rest_indices(
        order.data() + 1, order.size() - 1);
    EArrX xx1 = GetSubArray(x1, rest_indices).cwiseMax(x1[i]);
    EArrX yy1 = GetSubArray(y1, rest_indices).cwiseMax(y1[i]);
    EArrX xx2 = GetSubArray(x2, rest_indices).cwiseMin(x2[i]);
    EArrX yy2 = GetSubArray(y2, rest_indices).cwiseMin(y2[i]);

    EArrX w = (xx2 - xx1 + 1.0).cwiseMax(0.0);
    EArrX h = (yy2 - yy1 + 1.0).cwiseMax(0.0);
    EArrX inter = w * h;
    EArrX ovr = inter / (areas[i] + GetSubArray(areas, rest_indices) - inter);

    // indices for sub array order[1:n]
    auto inds = GetArrayIndices(ovr <= thresh);
    order = GetSubArray(order, AsEArrXt(inds) + 1);
  }

  return keep;
}

// Greedy non-maximum suppression for proposed bounding boxes
// Reject a bounding box if its region has an intersection-overunion (IoU)
//    overlap with a higher scoring selected bounding box larger than a
//    threshold.
// Reference: detection.caffe2/lib/nms/py_cpu_nms.py
// proposals: pixel coordinates of proposed bounding boxes,
//    size: (M, 4), format: [x1; y1; x2; y2]
// scores: scores for each bounding box, size: (M, 1)
// return: row indices of the selected proposals
template <class Derived1, class Derived2>
std::vector<int> nms_cpu(
    const Eigen::ArrayBase<Derived1>& proposals,
    const Eigen::ArrayBase<Derived2>& scores,
    float thres) {
  std::vector<int> indices(proposals.rows());
  std::iota(indices.begin(), indices.end(), 0);
  std::sort(
      indices.data(),
      indices.data() + indices.size(),
      [&scores](int lhs, int rhs) { return scores(lhs) > scores(rhs); });

  return nms_cpu(proposals, scores, indices, thres);
}

} // namespace utils
} // namespace caffe2

#endif // CAFFE2_OPERATORS_UTILS_NMS_H_
