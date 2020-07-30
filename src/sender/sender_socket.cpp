#include "sender/sender_socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
// #include <stdio.h>

#include <iostream>
#include <string>
#include <vector>

#ifdef DEBUG
// just for DEBUG
#include <fstream>
#endif

namespace udp_streaming_video {

SenderSocket::SenderSocket(
    const std::string &receiver_ip, const int receiver_port) {

  socket_handle_ = socket(AF_INET, SOCK_DGRAM, 0);
  receiver_addr_.sin_family = AF_INET;
  receiver_addr_.sin_port = htons(receiver_port);
  receiver_addr_.sin_addr.s_addr = inet_addr(receiver_ip.c_str());
}

void SenderSocket::SendPacket(
    const std::vector<unsigned char> &data,
    const int package_len,
    const int package_num) const {

  std::size_t end_index_t = 0;
  unsigned char index[4] = {0};
  int index_t = 0;

#ifdef DEBUG
  // just for DEBUG
  static int times_1 = 1;
  static int i_num = 0;
#endif

  for (int i = 0; i < package_num; i++)
  {
    if ((i+1)*package_len > data.size())
    {
      end_index_t = data.size();
    }
    else
    {
      end_index_t = (i+1)*package_len;
    }
    
    std::vector<unsigned char>::const_iterator begin_index = data.begin() + i*package_len;
    std::vector<unsigned char>::const_iterator end_index = data.begin() + end_index_t;
    std::vector<unsigned char> data_packaged(begin_index, end_index);

#ifdef DEBUG
  if (times_1)
  {
    std::cout << "0: data_packaged.size(): " << data_packaged.size() << std::endl;
  }
#endif

    // index << std::to_string(i) << std::endl;
    index_t = i;
    index[0] = (unsigned char)(index_t&0xFF);
    index[1] = (unsigned char)((index_t>>8)&0xFF); 
    index[2] = (unsigned char)((index_t>>16)&0xFF); 
    index[3] = (unsigned char)((index_t>>24)&0xFF); 

    // sprintf((char *)index, "%d", i);
    data_packaged.insert(data_packaged.begin(), index, index+4);

#ifdef DEBUG
  // if (times_1)
  // {
  //   std::cout << "0: data_packaged.size(): " << data_packaged.size() << ", index is: " \
  //   << (unsigned int)data_packaged[3] << " " \
  //   << (unsigned int)data_packaged[2] << " " \
  //   << (unsigned int)data_packaged[1] << " " \
  //   << (unsigned int)data_packaged[0] << " " \
  //   << std::endl;
  // }
#endif

    sendto(
        socket_handle_,
        data_packaged.data(),
        data_packaged.size(),
        0,
        const_cast<sockaddr*>(reinterpret_cast<const sockaddr*>(&receiver_addr_)),
        sizeof(receiver_addr_));

#ifdef DEBUG
    // just for DEBUG
    if (times_1)
    {
      std::ofstream outfile;
      outfile.open("image2.dat", std::ios::out | std::ios::app | std::ios::binary);

      for (size_t l = 0; l < 1200; l++)
      {
        outfile <<(unsigned int)((data_packaged[l]));
        i_num++;
        if ((i_num % (640*3)) == 0)
        {
          outfile << ";\r\n";
        }
        else
        {
          outfile << ", ";
        }
      }

      outfile.close();
    }
#endif

  }

#ifdef DEBUG
  // just for DEBUG
  if (times_1)
  {
      // std::cout << "data_packaged.size: " << data_packaged.size() << std::endl;
      std::cout << "total bytes in one picture: " << i_num << std::endl;
  }
  
  times_1 = 0;
#endif

}

}  // namespace udp_streaming_video
