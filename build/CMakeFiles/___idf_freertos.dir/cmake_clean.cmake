file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "project_elf_src.c"
  "text_over_lora.bin"
  "text_over_lora.map"
  "CMakeFiles/___idf_freertos"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/___idf_freertos.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
