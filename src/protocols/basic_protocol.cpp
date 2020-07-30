#include "protocols/basic_protocol.h"

#include <vector>
#include <iostream>

namespace udp_streaming_video {

std::vector<unsigned char> BasicProtocolData::PackageData() const {
  if (BasicProtocolData::picture_compress_flag_)
  {
    return video_frame_.GetJPEG();
  }
  else
  {
    return video_frame_.GetRaw();
  }
}

void BasicProtocolData::UnpackData(
    const std::vector<unsigned char>& raw_bytes) {

  video_frame_ = VideoFrame(raw_bytes);
}

BasicProtocolData::BasicProtocolData(const bool picture_compress_flag, const int package_len, const int picture_size) {
  picture_compress_flag_ = picture_compress_flag;
  package_len_ = package_len;
  if (picture_size % package_len)
  {
    package_num_ = picture_size / package_len + 1;
  }
  else
  {
    package_num_ = picture_size / package_len;
  }
}

}  // namespace udp_streaming_video
