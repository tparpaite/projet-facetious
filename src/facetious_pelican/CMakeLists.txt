add_library(facetious_pelican board.c position.c affect.c constraint.c ../z3.c)
target_link_libraries(facetious_pelican ADT)
install(FILES ${PROJECT_BINARY_DIR}/src/facetious_pelican/libfacetious_pelican.a DESTINATION ${CMAKE_LIBRARY_PATH})