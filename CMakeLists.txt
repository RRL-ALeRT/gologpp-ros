cmake_minimum_required(VERSION 3.5)
project(gologpp_agent)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_definitions(-DSOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")
# suppress all warnings
add_definitions(-w)

# Find and download the Eclipse CLP library
include(ExternalProject)
ExternalProject_Add(eclipse_clp
    URL https://eclipseclp.org/Distribution/Builds/7.1_13/x86_64_linux/eclipse_basic.tgz
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/eclipse_clp
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Set DOWNLOAD_EXTRACT_TIMESTAMP explicitly
)

# Set the include and library directories
set(ECLIPSE_CLP_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/eclipse_clp/src/eclipse_clp/include/x86_64_linux/)
set(ECLIPSE_CLP_LIBRARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/eclipse_clp/src/eclipse_clp/lib/x86_64_linux/)

# Add include and library directories
link_directories(lib ${ECLIPSE_CLP_LIBRARY_DIR})

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(builtin_interfaces REQUIRED)

set(gologpp_action_pkgs
	turtlesim
	gpp_action_examples_interface
	nav2_msgs
	webots_spot_msgs
	spot_msgs
)

set(found_action_pkgs)
foreach(pkg ${gologpp_action_pkgs})
	find_package(${pkg})
	if ("${${pkg}_FOUND}")
		list(APPEND found_action_pkgs ${pkg})
	endif()
endforeach()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations -Wno-placement-new")
endif()

set(BUILD_TESTS OFF CACHE BOOL "Enable gologpp tests" FORCE)

install(
	DIRECTORY ${ECLIPSE_CLP_LIBRARY_DIR}
	DESTINATION lib/eclipse
)

install(
	DIRECTORY lib/
	DESTINATION lib
)

# Create symlinks of libEclipse
macro(eclipse_lib_symlink filename)
  install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_INSTALL_PREFIX}/lib/eclipse/${filename} ${CMAKE_INSTALL_PREFIX}/lib/${filename})")
endmacro(eclipse_lib_symlink)

add_executable(gologpp_agent
	src/agent_node.cpp
	src/ros_backend.cpp
	src/action_manager.cpp
	src/exog_manager.cpp
)
add_dependencies(gologpp_agent eclipse_clp)
ament_target_dependencies(gologpp_agent rclcpp rclcpp_action rclcpp_components builtin_interfaces tf2 tf2_ros std_msgs std_srvs geometry_msgs ${found_action_pkgs})
target_link_libraries(gologpp_agent golog++ parsegolog++ readylog++ taptenc-golog++)
eclipse_lib_symlink(libeclipse.so)
target_include_directories(gologpp_agent PUBLIC
	src/
	include/golog++/
	${ECLIPSE_CLP_INCLUDE_DIR}
)

install(
	TARGETS gologpp_agent
	DESTINATION lib/${PROJECT_NAME}
)

foreach(pkg ${found_action_pkgs})
	target_sources(gologpp_agent PRIVATE "src/actions/gpp_${pkg}.cpp")
	target_compile_definitions(gologpp_agent PUBLIC "-D${pkg}_FOUND")
endforeach()

target_compile_features(
	gologpp_agent
	PUBLIC
	c_std_99
	cxx_std_17
)
ament_package()
