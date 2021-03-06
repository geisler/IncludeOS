// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <sys/socket.h>
#include <errno.h>
#include <fd_map.hpp>
#include <tcp_fd.hpp>
#include <udp_fd.hpp>

int socket(int domain, int type, int protocol)
{
  // disallow strange domains, like ALG
  if (domain < 0 || domain > AF_INET6) { errno = EAFNOSUPPORT; return -1; }
  // disallow RAW etc
  if (type < 0 || type > SOCK_DGRAM) { errno = EINVAL; return -1; }
  // we are purposefully ignoring the protocol argument
  if (protocol < 0) { errno = EPROTONOSUPPORT; return -1; }

  return [](const int type)->int{
    switch(type)
    {
      case SOCK_STREAM:
        return FD_map::_open<TCP_FD>().get_id();
      case SOCK_DGRAM:
        return FD_map::_open<UDP_FD>().get_id();
      default:
        return -1;
    }
  }(type);
}

int connect(int socket, const struct sockaddr *address, socklen_t len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.connect(address, len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}

ssize_t send(int socket, const void *message, size_t len, int fmt)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.send(message, len, fmt);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
ssize_t sendto(int socket, const void *message, size_t len, int flags,
  const struct sockaddr *dest_addr, socklen_t dest_len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.sendto(message, len, flags, dest_addr, dest_len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
ssize_t recv(int socket, void *buffer, size_t length, int flags)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.recv(buffer, length, flags);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
ssize_t recvfrom(int socket, void *buffer, size_t length,
  int flags, struct sockaddr *address, socklen_t *address_len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.recvfrom(buffer, length, flags, address, address_len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
int listen(int socket, int backlog)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.listen(backlog);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
int accept(int socket, struct sockaddr *address, socklen_t *len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.accept(address, len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
int bind(int socket, const struct sockaddr* address, socklen_t len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.bind(address, len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
int getsockopt(int socket, int level, int option_name,
  void *option_value, socklen_t *option_len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.getsockopt(level, option_name, option_value, option_len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
int setsockopt(int socket, int level, int option_name,
  const void *option_value, socklen_t option_len)
{
  try {
    auto& fd = FD_map::_get(socket);
    return fd.setsockopt(level, option_name, option_value, option_len);
  } catch (const FD_not_found&) {
    errno = EBADF;
    return -1;
  }
}
