#include "video/video_capture.h"

#include <iostream>
#include <vector>

#ifdef DEBUG
// just for DEBUG
#include <fstream>
#endif

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

namespace udp_streaming_video {

VideoCapture::VideoCapture(const bool show_video, const float scale, const int fps, const int width, const int height)
    : show_video_(show_video), scale_(scale), fps_(fps), width_(width), height_(height), capture_(cv::VideoCapture(0)) {
  
  capture_.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));
  capture_.set(cv::CAP_PROP_FPS, fps);
  capture_.set(cv::CAP_PROP_FRAME_WIDTH, width);
  capture_.set(cv::CAP_PROP_FRAME_HEIGHT, height);

  //打印摄像头参数
  std::cout << "width = " <<  capture_.get(cv::CAP_PROP_FRAME_WIDTH) << std::endl;
  std::cout << "height = " <<  capture_.get(cv::CAP_PROP_FRAME_HEIGHT) << std::endl;
  std::cout << "fbs = " <<  capture_.get(cv::CAP_PROP_FPS) << std::endl;
  std::cout << "brightness = " <<  capture_.get(cv::CAP_PROP_BRIGHTNESS) << std::endl;
  std::cout << "contrast = " <<  capture_.get(cv::CAP_PROP_CONTRAST) << std::endl;
  std::cout << "saturation = " <<  capture_.get(cv::CAP_PROP_SATURATION) << std::endl;
  std::cout << "hue = " <<  capture_.get(cv::CAP_PROP_HUE) << std::endl;
  int codec = int(capture_.get(cv::CAP_PROP_FOURCC));
  std::cout << "encode format = " <<  char(codec&0xFF) << char((codec>>8)&0xFF) << char((codec>>16)&0xFF) << char((codec>>24)&0xFF)  << std::endl;
  std::cout << "port format = " << capture_.get(cv::CAP_PROP_FORMAT) << std::endl;

  // TODO: Verify that the scale is in the appropriate range.
}

VideoFrame VideoCapture::GetFrameFromCamera() {
  if (!capture_.isOpened()) {
    std::cerr << "Could not get frame. Camera not available." << std::endl;
    return VideoFrame();
  }
  cv::Mat image;
  capture_ >> image;

#ifdef DEBUG
  // just for DEBUG
  static int times = 1;
  if (times)
  {
    std::ofstream outfile;
    outfile.open("image.dat", std::ios::out | std::ios::trunc );
    outfile << image;
    outfile.close();
    times = 0;
    std::cout << "write mat to file" << std::endl;
    std::cout << "image.type(): " << image.type() << std::endl;
    std::cout << "CV_16SC3: " << CV_16SC3 << std::endl;
    std::cout << "image.size(): " << image.size() << std::endl;
    std::cout << "image.channels(): " << image.channels() << std::endl;
    std::cout << "image.depth(): " << image.depth() << std::endl;
    std::cout << "image.step(): " << image.step << std::endl;
    std::cout << "image.step1(): " << image.step1() << std::endl;
  }
#endif

  // If the image is being downsampled, resize it first.
  if (scale_ < 1.0) {
    cv::resize(image, image, cv::Size(0, 0), scale_, scale_);
  }
  VideoFrame video_frame(image);
  if (show_video_) {
    video_frame.Display();
  }
  return video_frame;
}


  int VideoCapture::GetFps() const {
    return capture_.get(cv::CAP_PROP_FPS);
  }
  int VideoCapture::GetWidth() const {
    return capture_.get(cv::CAP_PROP_FRAME_WIDTH);
  }
  int VideoCapture::GetHeight() const {
    return capture_.get(cv::CAP_PROP_FRAME_HEIGHT);
  }
  int VideoCapture::GetChannel() const {
    return 3;
  }

}  // namespace udp_streaming_video
