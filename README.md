# Streaming UDP Video

This is a simple video streaming application. The sender captures video frames from a webcam using OpenCV, and sends the frames to the receiver over UDP. The frames are compressed in JPEG format and optionally downsampled to reduce the data payload. The receiver decodes the JPEG data and displays the video frame in a GUI window (also using OpenCV).

Requires OpenCV 2. Works on OS X and Linux.

# What's new

In order to verify some image algorithms more conveniently, I need to realize the original image transmission through the network on the platform of FPGA. In order to obtain the uncompressed image, the original image transmission mode is added. The data-rate of 640 * 480 * 24bit image is about 26MBps. Test on [PYNQ-Z2](https://pynq.io/) is passed.

TODO
------

- [x] Test the build on Linux.
- [x] UDP packets that are too large won't be delivered. Split up frame packets that exceed a certain size limit.
- [x] Test the build on Linux for ARM 32 bit.
- [ ] There is currently no security, and anyone can send a packet to the receiver, whether it's a video frame or not. Implement a protocol that adds some security and ensures the incoming data is valid. Possibly encrypt the video.