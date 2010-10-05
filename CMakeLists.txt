cmake_minimum_required(VERSION 2.6)
project(ARPWATCH-V6)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(optionalLibs)

find_package(PCAP)
IF(PCAP_FOUND)
	SET(optionalLibs ${optionalLibs} ${PCAP_LIBRARIES})
ENDIF(PCAP_FOUND)

add_executable(arpwatch-v6 arpwatch-v6.c)
target_link_libraries(arpwatch-v6 ${optionalLibs})
