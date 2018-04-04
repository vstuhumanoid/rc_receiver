cmake_minimum_required(VERSION 3.5.1)
project(rc_receiver)

cmake_policy(SET CMP0071 NEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 14)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  message_generation
)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets)

qt5_wrap_ui(RC_RECEIVER_QT_UI_HEADERS src/mainwindow.ui)

add_message_files(
  FILES
  PultData.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rc_receiver
 CATKIN_DEPENDS roscpp std_msgs message_runtime
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

add_executable(${PROJECT_NAME}_node src/rc_receiver_node.cpp
        src/mainwindow.cpp ${RC_RECEIVER_QT_UI_HEADERS} src/pultreceiver.cpp src/packagebuilder.cpp)

# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
#  ${Qt5Core_LIBRARIES} ${Qt5Widgets_LIBRARIES}
)

qt5_use_modules(rc_receiver_node Core Widgets SerialPort)
