include(CheckLibraryExists)
include(CheckIncludeFiles)

cmake_minimum_required(VERSION 2.8.9)
project (nrf24le1_flasher)

### Check Dependencies
find_package(PkgConfig REQUIRED)

# Check if libusb-1.0 is installed
check_library_exists("usb-1.0" "libusb_init" "x86_64-linux-gnu/" HAVE_LIBUSB)
IF(NOT HAVE_LIBUSB)
  message( FATAL_ERROR "LIBUSB is not found" )
ENDIF()
check_include_files ("libusb-1.0/libusb.h" HAVE_LIBUSB_H)
IF(NOT HAVE_LIBUSB_H)
  message( FATAL_ERROR "LIBUSB is not found" )
ENDIF()
pkg_search_module(USB REQUIRED libusb-1.0)

# Check if ftdi is installed
check_library_exists("ftdi" "ftdi_init" "x86_64-linux-gnu/" HAVE_FTDI)
IF(NOT HAVE_FTDI)
  message( FATAL_ERROR "FTDI is not found" )
ENDIF()
check_include_files (ftdi.h HAVE_FTDI_H)
IF(NOT HAVE_FTDI_H)
  message( FATAL_ERROR "FTDI is not found" )
ENDIF()
pkg_search_module(FTDI REQUIRED libftdi)

include_directories(".")
set(EXEC_SRCS hexfile.c
              spi_ftmpsse.c
              main.c
)

### Testapp
add_executable(nrf24le1_flasher ${EXEC_SRCS})
target_link_libraries(nrf24le1_flasher ${FTDI_LIBRARIES} ${USB_LIBRARIES} mpsse)
install(TARGETS nrf24le1_flasher RUNTIME DESTINATION bin)
