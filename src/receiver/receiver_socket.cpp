#include "receiver/receiver_socket.h"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

#include <iostream>
#include <vector>

namespace udp_streaming_video {

ReceiverSocket::ReceiverSocket(const int port_number) : port_(port_number) {
  socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
  start_image_flag = 0;
  data_buf.reserve(640*480*3);
  data_buf.resize(640*480*3);
  data_buf.assign(640*480*3, 0);
#ifdef DEBUG
  std::cout << "data_buf.size() is: " << data_buf.size() << std::endl;
#endif
}

const bool ReceiverSocket::BindSocketToListen() const {
  if (socket_handle_ < 0) {
    std::cerr << "Binding failed. Socket was not initialized." << std::endl;
    return false;
  }
  // Bind socket's address to INADDR_ANY because it's only receiving data, and
  // does not need a valid address.
  sockaddr_in socket_addr;
  memset(reinterpret_cast<char*>(&socket_addr), 0, sizeof(socket_addr));
  socket_addr.sin_family = AF_INET;
  socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  // Bind the socket to the port that will be listened on.
  socket_addr.sin_port = htons(port_);
  const int bind_status = bind(
      socket_handle_,
      reinterpret_cast<sockaddr*>(&socket_addr),
      sizeof(socket_addr));
  if (bind_status < 0) {
    std::cerr << "Binding failed. Could not bind the socket." << std::endl;
    return false;
  }
  return true;
}

const std::vector<unsigned char> ReceiverSocket::GetPacket(
    const int package_len,
    const int package_num) const {
  // Get the data from the next incoming packet.
  sockaddr_in remote_addr;
  socklen_t addrlen = sizeof(remote_addr);
  // std::vector<unsigned char> data;

  int index_t = 0;
  // unsigned char index[4] = {0};
  // index[0] = (unsigned char)((package_num-1)&0xFF);
  // index[1] = (unsigned char)(((package_num-1)>>8)&0xFF); 
  // index[2] = (unsigned char)(((package_num-1)>>16)&0xFF); 
  // index[3] = (unsigned char)(((package_num-1)>>24)&0xFF); 

  while(true) {
    const int num_bytes = recvfrom(
        socket_handle_,
        const_cast<void*>(reinterpret_cast<const void*>(buffer_)),
        kMaxPacketBufferSize,
        0,
        reinterpret_cast<sockaddr*>(&remote_addr),
        &addrlen);
    // Copy the data (if any) into the data vector.
    if (num_bytes > 0) {
      index_t = (int)buffer_[0] | ((int)buffer_[1])<<8 | ((int)buffer_[2])<<16 | ((int)buffer_[3])<<24;
      
      std::copy((buffer_+4), \
        (buffer_+num_bytes), \
        (data_buf.begin()+index_t*package_len));

#ifdef DEBUG
      std::cout << "data_buf.size() is: " << data_buf.size() << " index is: " << index_t <<std::endl;
#endif

      if (index_t == package_num-1)
      {
        // std::cout << "one frame! "<< data_buf.size() << std::endl;
        break;
      }
    }
  }
  // std::vector<unsigned char> data(data_buf);
  return data_buf;
}

}  // namespace udp_streaming_video
