cmake_minimum_required(VERSION 2.8.9)
project (nrf24le1_flasher)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

add_subdirectory(src)

### Debian Package generation
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Franz Flasch") #required
include(CPack)
