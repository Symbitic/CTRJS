################################################################################
# This file is part of CTRJS.
# 
# Copyright © 2015 Symbitic.
# 
# CTRJS is free software: you can redistribute it and/or modify it 
# under the terms of version 3 of the GNU General Public License, as published
# by the Free Software Foundation.
# 
# CTRJS is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
# more details.
# 
# You should have received a copy of the GNU General Public License along with
# CTRJS. If not, see <http://www.gnu.org/licenses/>.
################################################################################

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

set(CMAKE_BUILD_TYPE_INIT "Debug")

project(CTRJS)

set(CTRJS_VERSION 0.1.0)

find_package(OpenCTR CONFIG REQUIRED)
find_package(libctr CONFIG REQUIRED)
find_library(MATH_LIBRARY m)

option(ENABLE_DOC "Enable documentation" OFF)

if(ENABLE_DOC)
	add_subdirectory(doc)
endif()

add_subdirectory(data)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_definitions(-DNS_DISABLE_NETWORK)
add_definitions(-DNS_DISABLE_THREAD)
add_definitions(-DV7_BUILD_PROFILE=1)

add_executable(ctrjs
	src/main.c
	src/v7.c
)

target_link_libraries(ctrjs
	CTR::CTR
	${MATH_LIBRARY}
)

ctr_convert_binary(ctrjs)
