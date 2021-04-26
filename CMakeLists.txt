cmake_minimum_required (VERSION 3.10)

project (DRTFace)

# The version number
set (DRTFACE_VERSION_MAJOR 1)
set (DRTFACE_VERSION_MINOR 0)
set (DRTFACE_VERSION_PATCH 0)
set (DRTFACE_VERSION ${DRTFACE_VERSION_MAJOR}.${DRTFACE_VERSION_MINOR}.${DRTFACE_VERSION_PATCH})

configure_file(${PROJECT_SOURCE_DIR}/cmake/types.h.in ${PROJECT_SOURCE_DIR}/include/types.h)

# OpenCV
find_package(OpenCV 4.4 REQUIRED core features2d objdetect highgui imgproc imgcodecs ml dnn PATHS "$ENV{OPENCVDIR}/build")
# find_package(OpenCV 3 REQUIRED PATHS "$ENV{OPENCVDIR}/build")

# Turn on the ability to create folders to organize projects (.vcproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake defined projects like INSTALL.vcproj and ZERO_CHECK.vcproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Sets
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "")
if(WIN32)
set(LIB_SUFFIX ".lib")
set(BAT_SUFFIX ".bat") 
else()
if (APPLE)
set(LIB_SUFFIX ".dylib")	# in MacOS
else()
set(LIB_SUFFIX ".so")		# in Linux
endif(APPLE)
set(BAT_SUFFIX ".sh")
endif(WIN32)
set(CORE_LIB $<$<CONFIG:Debug>:drtface_core${DRTFACE_VERSION_MAJOR}${DRTFACE_VERSION_MINOR}${DRTFACE_VERSION_PATCH}${CMAKE_DEBUG_POSTFIX}${LIB_SUFFIX}>$<$<CONFIG:Release>:drtface_core${DRTFACE_VERSION_MAJOR}${DRTFACE_VERSION_MINOR}${DRTFACE_VERSION_PATCH}${LIB_SUFFIX}>)

set(CMAKE_CXX_STANDARD 17)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /wd5040")
set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF")					
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF")
endif(MSVC)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
set(CMAKE_INSTALL_PREFIX ${PROJECT_BINARY_DIR}/install CACHE PATH "Installation directory")

#install
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/data DESTINATION ${CMAKE_INSTALL_PREFIX})
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/etc DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${PROJECT_SOURCE_DIR}/License.txt DESTINATION ${CMAKE_INSTALL_PREFIX})
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/batch/ DESTINATION ${CMAKE_INSTALL_PREFIX}/bin FILES_MATCHING PATTERN "*${BAT_SUFFIX}" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
set(CMAKE_MACOSX_RPATH ON)																		# MacOS	
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" "${OpenCV_INSTALL_PATH}/lib" "../lib")	# MacOS

#definitions
add_definitions(-D_CRT_SECURE_NO_WARNINGS 
		        -D_SCL_SECURE_NO_WARNINGS
		        )

# Options
include(CMakeDependentOption)
#option(DEBUG_PRINT_INFO "Output debug information" OFF)
#option(DEBUG_MODE "Debugging mode" OFF)
#option(ENABLE_PDP "Use parallel data processing" ON)
#cmake_dependent_option(ENABLE_AMP "Use AMP Algorithms Library for parallel GPU computing" OFF "MSVC" OFF)  
#option(ENABLE_BSP "Use Binary Space Partitioning (BSP) Tree for optimized ray traversal" ON)

# Sub-directories where more CMakeLists.txt exist
add_subdirectory(modules/core)
#add_subdirectory(tests)
add_subdirectory(demos)

# ===============================

# Add all targets to the build-tree export set
export(TARGETS core FILE ${PROJECT_BINARY_DIR}/DRTFaceTargets.cmake)

# Export the package for use from the build-tree
export(PACKAGE DRTFACE)

# Create the FooBarConfig.cmake and FooBarConfigVersion files
# REL_INCLUDE_DIR = include
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(${PROJECT_SOURCE_DIR}/cmake/DRTFaceConfig.cmake.in ${PROJECT_BINARY_DIR}/DRTFaceConfig.cmake @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${DRTFACE_CMAKE_DIR}/include")
configure_file(${PROJECT_SOURCE_DIR}/cmake/DRTFaceConfig.cmake.in ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DRTFaceConfig.cmake @ONLY)
# ... for both
configure_file(${PROJECT_SOURCE_DIR}/cmake/DRTFaceConfigVersion.cmake.in ${PROJECT_BINARY_DIR}/DRTFaceConfigVersion.cmake @ONLY)
 
# Install the FooBarConfig.cmake and FooBarConfigVersion.cmake
install(FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/DRTFaceConfig.cmake ${PROJECT_BINARY_DIR}/DRTFaceConfigVersion.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
 
# Install the export set for use with the install-tree
install(EXPORT DRTFaceTargets DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

