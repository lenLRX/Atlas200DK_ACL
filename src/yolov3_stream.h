#ifndef __YOLOV3_STREAM_H__
#define __YOLOV3_STREAM_H__

#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "acl_model.h"
#include "app_profiler.h"

using json = nlohmann::json;

class Yolov3Model {
public:
  // input type: <raw image, resized image>
  using InTy = std::tuple<DeviceBufferPtr, DeviceBufferPtr>;
  // output type: <box info, raw image>
  using OutTy = std::tuple<ACLModel::DevBufferVec, DeviceBufferPtr>;

  Yolov3Model(const std::string &path, aclrtStream stream);
  OutTy Process(InTy bufferx2);

private:
  ACLModel yolov3_model;
  aclrtStream model_stream;
};

class Yolov3PostProcess {
public:
  using InTy = std::tuple<ACLModel::DevBufferVec, DeviceBufferPtr>;
  using OutTy = DeviceBufferPtr;
  Yolov3PostProcess(int width, int height);
  OutTy Process(InTy input);

private:
  int width;
  int height;
  float h_ratio;
  float w_ratio;
};

std::thread MakeYolov3Stream(json config);

#endif //__YOLOV3_STREAM_H__