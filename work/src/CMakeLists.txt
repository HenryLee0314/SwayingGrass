
# Add all source Files in dir
file(GLOB sources *.c *.cpp *.h *.hpp)

list(APPEND sources
    "CMakeLists.txt"
)

# Add executable target and link libraries
add_executable(${CGRA_PROJECT} ${sources})

# ----TODO--------------------- #
# list your subdirectories here #
# ----------------------------- #
add_subdirectory(cgra)
add_subdirectory(YHL_base)
add_subdirectory(CGRA350)

#########################################################
# Link and Build Executable
#########################################################

# Set source groups (helper method)
target_source_group_tree(${CGRA_PROJECT})

# Set working folder in the code for the shaders
target_compile_definitions(${CGRA_PROJECT} PRIVATE "-DCGRA_SRCDIR=\"${PROJECT_SOURCE_DIR}\"")

# Link usage requirements
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stb imgui)
target_link_libraries(${CGRA_PROJECT} PRIVATE ${OpenCL_LIBRARY})

# For experimental <filesystem>
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(${CGRA_PROJECT} PRIVATE -lstdc++fs)
endif()
