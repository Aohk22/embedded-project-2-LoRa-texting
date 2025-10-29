# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/tkl/my_files/projects_embedded/text_over_lora/esp/ESP8266_RTOS_SDK/components/bootloader/subproject"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/tmp"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/src/bootloader-stamp"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/src"
  "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/tkl/my_files/projects_embedded/text_over_lora/source_code/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
