add_executable(${CMAKE_PROJECT_NAME} utils.cpp framework.cpp image.cpp
	application.cpp main.cpp draw.cpp gui.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${OPENGL_LIBRARIES}
	${GLUT_LIBRARIES} ${SDL2_LIBRARIES})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR})
