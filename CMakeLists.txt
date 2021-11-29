cmake_minimum_required(VERSION 3.0)

project(proto-test)

if (MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -g)
endif()
	
#Add the path to new folders with files and go to the folder build and call cmake ..
file(GLOB project_src 
	"source/src/behavior/*.c"
	"source/src/input/*.c"
	"source/src/misc/*.c"
	"source/src/parse/*.c"
	"source/src/screen/*.c"
	"source/src/sound/*.c"
	"source/src/sprite/*.c"
	"source/src/sprite/platformer/*.c"
	"source/src/sprite/shooter/*.c"
	)
	
#Add the path to new folders with files and go to the folder build and call cmake ..
include_directories(
	"/usr/include/SDL2" # Here you put the path to SDL2/image/mixer include files.  
	"source/include/behavior"
	"source/include/input"
	"source/include/misc"
	"source/include/parse"
	"source/include/screen"
	"source/include/sound"
	"source/include/sprite"
	"source/include/sprite/platformer"
	"source/include/sprite/shooter"
)
	
	
add_executable(prot ${project_src})

set_property(TARGET prot PROPERTY C_STANDARD 11)

if (WIN32)
	target_link_libraries(prot SDL2main SDL2 SDL2_image SDL2_mixer)
else()
	target_link_libraries(prot SDL2 SDL2_image SDL2_mixer)
endif()







