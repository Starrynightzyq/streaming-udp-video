#include "video/video_frame.h"

#include <string>
#include <vector>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs/legacy/constants_c.h"

namespace udp_streaming_video {
namespace {

// The name of the window in which all frames will be displayed. If a new frame
// is displayed, it will replace the previous frame displayed in that window.
static const std::string kWindowName = "Streaming Video";

// Delays thread execution for this amount of time after a frame is displayed.
// This prevents the window from being refreshed too often, which can cause
// display issues.
constexpr int kDisplayDelayTimeMS = 15;

// JPEG compression values.
static const std::string kJPEGExtension = ".jpg";
constexpr int kJPEGQuality = 25;

}  // namespace

VideoFrame::VideoFrame(const std::vector<unsigned char> frame_bytes) {
  cv::Mat img(cv::Size(640, 480), 16, (void*)frame_bytes.data());
  frame_image_ = img;
  // frame_image_ = cv::imdecode(img, CV_LOAD_IMAGE_COLOR);
  // frame_image_ = cv::imdecode(frame_bytes, cv::LOAD_IMAGE_COLOR);
}

void VideoFrame::Display() const {
  // Do nothing for empty images.
  if (frame_image_.empty()) {
    return;
  }
  cv::imshow(kWindowName, frame_image_);
  cv::waitKey(kDisplayDelayTimeMS);
}

std::vector<unsigned char> VideoFrame::GetJPEG() const {
  const std::vector<int> compression_params = {
      cv::IMWRITE_JPEG_QUALITY,
      kJPEGQuality
  };
  std::vector<unsigned char> data_buffer;
  cv::imencode(kJPEGExtension, frame_image_, data_buffer, compression_params);
  return data_buffer;
}

std::vector<unsigned char> VideoFrame::GetRaw() const {
  const unsigned char* p = (const unsigned char*)frame_image_.data;
  std::vector<unsigned char> array(p, p+frame_image_.cols*frame_image_.rows*frame_image_.channels());
  return array;
}

int VideoFrame::GetMatSize() const {
  return frame_image_.rows*frame_image_.cols;
}

}  // namespace udp_streaming_video
