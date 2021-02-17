file(GLOB CORE_INCLUDE  ${PROJECT_SOURCE_DIR}/include/drtface.h
                        ${PROJECT_SOURCE_DIR}/include/types.h
                        ${PROJECT_SOURCE_DIR}/include/macroses.h
						)
file(GLOB CORE_SOURCES "*.cpp")
file(GLOB CORE_HEADERS "*.h")

source_group("Include" FILES ${CORE_INCLUDE})
source_group("" FILES ${CORE_SOURCES} ${CORE_HEADERS}) 
source_group("Source Files\\Face" FILES "Face.h" "Face.cpp")
source_group("Source Files\\Marker" FILES "Marker.h" "Marker.cpp")
source_group("Source Files\\Camera Controller" FILES "CameraController.h" "CameraController.cpp")

# Properties -> C/C++ -> General -> Additional Include Directories
include_directories(${PROJECT_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS})

add_library(core SHARED ${CORE_INCLUDE} ${CORE_SOURCES} ${CORE_HEADERS})

# Properties -> Linker -> Input -> Additional Dependencies
target_link_libraries(core ${OpenCV_LIBS})

set_target_properties(core PROPERTIES OUTPUT_NAME drtface_core${DRTFACE_VERSION_MAJOR}${DRTFACE_VERSION_MINOR}${DRTFACE_VERSION_PATCH})
set_target_properties(core PROPERTIES VERSION ${DRTFACE_VERSION_MAJOR}.${DRTFACE_VERSION_MINOR}.${DRTFACE_VERSION_PATCH} SOVERSION ${DRTFACE_VERSION_MAJOR}.${DRTFACE_VERSION_MINOR}.${DRTFACE_VERSION_PATCH})
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)

#install
install(TARGETS core 
	EXPORT DRTFaceTargets
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin 
	LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
	ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES ${CORE_INCLUDE} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install(FILES ${CORE_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/drtface)
 
# Creates folder "Modules" and adds target project 
set_target_properties(core PROPERTIES FOLDER "Modules")