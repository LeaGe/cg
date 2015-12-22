# Define project
cmake_minimum_required(VERSION 3.0)
project(CG1Exercise6)

include(common.cmake)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER "CMakeAutomocTargets")

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set target name
set(target61 CG1Exercise61)
set(target62 CG1Exercise62)
set(target63 CG1Exercise63)

find_package(OpenGL REQUIRED)

# Include QT5 libraries
find_package(Qt5Gui 5.4 REQUIRED)

# Define source files
set(util_sources	
	src/util/box.cpp
	src/util/box.h
    src/util/objio.cpp
    src/util/objio.h
    src/util/polygonaldrawable.cpp
    src/util/polygonaldrawable.h
    src/util/abstractexercise.cpp
    src/util/abstractexercise.h
    src/util/abstractpainter.h
    src/util/abstractpainter.cpp
    src/util/camera.h
    src/util/camera.cpp
    src/util/glviewer.h
    src/util/glviewer.cpp
    src/util/icosahedron.cpp
    src/util/icosahedron.h
    src/util/openglfunctions.h
    src/util/paintermode.h
    src/util/unitcube.cpp
    src/util/unitcube.h
)

set(target61_sources
    src/exercise61.cpp
	src/exercise61.h
)

set(target62_sources
	data/model.frag
	data/model.geom
	data/model.vert
	src/exercise62.cpp
	src/exercise62.h
)

set(target63_sources
	data/cube.frag
	data/cube.tcs
	data/cube.tes
	data/cube.geom
	data/cube.vert
    src/exercise63.cpp
	src/exercise63.h
)

# Create util library
add_library(util STATIC ${util_sources})

# Create executable
add_executable(${target61} ${target61_sources})
add_executable(${target62} ${target62_sources})
add_executable(${target63} ${target63_sources})

create_source_groups(${util_sources})
create_source_groups(${target61_sources})
create_source_groups(${target62_sources})
create_source_groups(${target63_sources})

# Create executable
include_directories(
    ${CMAKE_SOURCE_DIR}
)

# link libraries
target_link_libraries(util Qt5::Gui)
target_link_libraries(${target61} util Qt5::Gui ${OPENGL_LIBRARIES})
target_link_libraries(${target62} util Qt5::Gui ${OPENGL_LIBRARIES})
target_link_libraries(${target63} util Qt5::Gui ${OPENGL_LIBRARIES})
#${OPENGL_LIBRARIES}
