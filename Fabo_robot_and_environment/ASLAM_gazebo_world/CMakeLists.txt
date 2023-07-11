cmake_minimum_required(VERSION 3.0.2)
project(ASLAM_gazebo_world)
SET(CMAKE_BUILD_TYPE Debug)
## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  gazebo_msgs

  sensor_msgs

  geometry_msgs
  tf
  gazebo_ros
)

# set(Boost_INCLUDE_DIR /usr/include/boost)
# set(Boost_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)
find_package(Boost REQUIRED COMPONENTS filesystem thread date_time iostreams serialization chrono system)
find_package(gazebo REQUIRED)

# find_package(gazebo REQUIRED)
catkin_package(
  # INCLUDE_DIRS include
  # LIBRARIES ${PROJECT_NAME}
  # CATKIN_DEPENDS roscpp std_msgs gazebo_ros
  # INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
  # CATKIN_DEPENDS roscpp std_msgs test_pkg gazebo_msgs gazebo_ros
  # DEPENDS system_lib
)
find_package(Eigen3  REQUIRED)
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIR}
)

link_directories(${GAZEBO_LIBRARY_DIRS})
add_definitions(${GAZEBO_CXX_FLAGS})

# add_library(${PROJECT_NAME} src/gazebo_world_parser.cpp)
# target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

# add_executable(${PROJECT_NAME}_node src/gazebo_world_parser_node.cpp)
# target_link_libraries(${PROJECT_NAME}_node ${PROJECT_NAME} ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})


# add_executable(gazebo_world_parser src/gazebo_world_parser.cpp)
# target_link_libraries(gazebo_world_parser 
#   ${catkin_LIBRARIES} 
#   ${GAZEBO_LIBRARIES} 
# #  /usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1
#   )


add_executable(gazebo_object_rviz src/gazebo_object_rviz.cpp)
target_link_libraries(gazebo_object_rviz 
  ${catkin_LIBRARIES}
   ${GAZEBO_LIBRARIES}
   ${EIGEN3_LIBS}
#  /usr/lib/x86_64-linux-gnu/libboost_system.so.1.65.1
  )