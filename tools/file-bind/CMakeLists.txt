cmake_minimum_required(VERSION 3.0)


project(file-bind)


file(GLOB project_src
	"source/src/*.c"
)

include_directories(
	"source/include/"
)

add_executable(${PROJECT_NAME} ${project_src})

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)
