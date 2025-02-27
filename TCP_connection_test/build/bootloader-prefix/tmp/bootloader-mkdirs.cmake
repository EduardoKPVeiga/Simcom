# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/epont/esp/esp-idf/components/bootloader/subproject"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/tmp"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/src/bootloader-stamp"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/src"
  "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/repositories/TmDev_TCP_connection_test/TCP_connection_test/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
