# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/Qt6Hello_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Qt6Hello_autogen.dir/ParseCache.txt"
  "Qt6Hello_autogen"
  )
endif()
