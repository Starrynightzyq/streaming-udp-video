// The basic protocol transmits raw, uncompressed video frames only. This is
// the most basic video protocol.

#ifndef SRC_PROTOCOLS_BASIC_PROTOCOL_H_
#define SRC_PROTOCOLS_BASIC_PROTOCOL_H_

#include <vector>

#include "protocols/protocol.h"
#include "video/video_frame.h"

namespace udp_streaming_video {

class BasicProtocolData : public ProtocolData {
 public:

  // BasicProtocolData(const bool picture_compress_flag) {
  //   picture_compress_flag_ = picture_compress_flag;
  // }

  BasicProtocolData(const bool picture_compress_flag, const int package_len, const int picture_size);

  std::vector<unsigned char> PackageData() const override;

  void UnpackData(
      const std::vector<unsigned char>& raw_bytes) override;

  // Sets the next video frame.
  void SetImage(const VideoFrame& image) {
    video_frame_ = image;
  }

  // Returns the video frame image.
  VideoFrame GetImage() const {
    return video_frame_;
  }

  bool GetCompressFlag() const {
    return picture_compress_flag_;
  }

  int GetPackageLen() const {
    return package_len_;
  }

  int GetPackageNum() const {
    return package_num_;
  }

 private:
  // The video frame received from the packet is stored here.
  VideoFrame video_frame_;
  bool picture_compress_flag_;
  int package_len_;
  int package_num_;
};

}  // namespace udp_streaming_video

#endif  // SRC_PROTOCOLS_BASIC_PROTOCOL_H_
