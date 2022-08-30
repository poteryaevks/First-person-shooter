set(TARGET_NAME client)

file(GLOB _SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
    )

file (GLOB _HEADER_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )

add_executable(${TARGET_NAME}
    "${_SOURCE_FILES}"
    "${_HEADER_FILES}"
    )


set_target_properties(${TARGET_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    )


target_include_directories(${TARGET_NAME} PRIVATE
    ${Almaz_INCLUDE_DIRS}
    ${NET_SHOOTER_ROOT_DIR}
    )

target_link_libraries(${TARGET_NAME} PUBLIC
    Almaz::logger
    Almaz::misc
    )

if(MINGW)

    target_compile_definitions(${TARGET_NAME} PRIVATE
        -D_USE_MATH_DEFINES
        -DUNICODE
        -DWIN32_LEAN_AND_MEAN
        -DBOOST_BIND_GLOBAL_PLACEHOLDERS
        )

    target_link_libraries(${TARGET_NAME} PUBLIC
        ws2_32
        wsock32
        )
endif()

