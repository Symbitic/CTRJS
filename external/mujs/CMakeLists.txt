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

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/astnames.h 
	COMMAND grep -E "(AST|EXP|STM)_" ${CMAKE_CURRENT_SOURCE_DIR}/src/jsparse.h | sed 's/^[^A-Z]*\(AST_\)*/\\"/;s/,.*/\\",/' | tr A-Z a-z > astnames.h
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/jsparse.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating astnames.h"
	VERBATIM
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/opnames.h
	COMMAND grep -E 'OP_' ${CMAKE_CURRENT_SOURCE_DIR}/src/jscompile.h | sed 's/^[^A-Z]*OP_/"/;s/,.*/",/' | tr A-Z a-z > opnames.h
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/jscompile.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Generating opnames.h"
	VERBATIM
)

set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/astnames.h PROPERTIES GENERATED TRUE)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/opnames.h PROPERTIES GENERATED TRUE)

find_library(MATH_LIBRARY m)

add_library(mujs 
	src/jsarray.c
	src/jsboolean.c
	src/jsbuiltin.c
	src/jscompile.c
	src/jsdate.c
	src/jsdtoa.c
	src/jsdump.c
	src/jserror.c
	src/jsfunction.c
	src/jsgc.c
	src/jsintern.c
	src/jslex.c
	src/jsmath.c
	src/jsnumber.c
	src/jsobject.c
	src/json.c
	src/jsparse.c
	src/jsproperty.c
	src/jsregexp.c
	src/jsrun.c
	src/jsstate.c
	src/jsstring.c
	src/jsvalue.c
	src/regex.c
	src/utf.c
	src/utftype.c
	${CMAKE_CURRENT_BINARY_DIR}/astnames.h
	${CMAKE_CURRENT_BINARY_DIR}/opnames.h
)

target_link_libraries(mujs PUBLIC
	${MATH_LIBRARY}
)

target_include_directories(mujs PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

