# Properties -> C/C++ -> General -> Additional Include Directories
include_directories(${PROJECT_SOURCE_DIR}/include 
					${PROJECT_SOURCE_DIR}/modules
					${OpenCV_INCLUDE_DIRS}
					${DLIB_INCLUDE_DIRS}
				)

# Properties -> Linker -> General -> Additional Library Directories
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

function(create_app target source)
	add_executable(${target} "${source}.cpp")
	add_dependencies(${target} core)

	source_group("Source Files" FILES "${source}.cpp")

	set_target_properties(${target} PROPERTIES PROJECT_LABEL "${source}")						# in Visual Studio
	set_target_properties(${target} PROPERTIES OUTPUT_NAME "${source}")
	set_target_properties(${target} PROPERTIES FOLDER "ML")

	target_include_directories(${target} PRIVATE ./include $ENV{HOME}/libtensorflow2/include)
	# Properties->Linker->Input->Additional Dependencies
	target_link_libraries(${target} ${OpenCV_LIBS} ${CORE_LIB} ${DLIB_LIBRARIES} dlib::dlib "${TENSORFLOW_LIB}")
	# --------
	#install
	install(TARGETS ${target} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endfunction()

# Add Demos
create_app(test "test")