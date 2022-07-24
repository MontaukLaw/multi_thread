add_library(
        demo
        STATIC
        demo.c demo.h)

target_include_directories(demo
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )


#target_sources(demo
#        PRIVATE
#        ${CMAKE_CURRENT_LIST_DIR}/demo.c
#        PUBLIC
#        ${CMAKE_CURRENT_LIST_DIR}/demo.h
#        )
