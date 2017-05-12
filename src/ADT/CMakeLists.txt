add_library(ADT list.c queue.c custom_type.c)
install(FILES ${PROJECT_BINARY_DIR}/src/ADT/libADT.a DESTINATION ${CMAKE_LIBRARY_PATH})