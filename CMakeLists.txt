cmake_minimum_required(VERSION 2.6)
project(ARPWATCH-V6)

option(WITH_MYSQL "build with MySQL support" ON)

set(SOURCES
	main.c
	db.c
	db/db_file.c
	capture.c
	config.c
	utils.c
	notify.c
	notify/notify_syslog.c
	notify/notify_email.c
)

add_subdirectory(db)
add_subdirectory(notify)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(optionalLibs)

set(CMAKE_C_FLAGS "-g -Wall")

find_package(PCAP)

IF(WITH_MYSQL)
	find_package(MySQL)
	IF(MYSQL_FOUND)
		ADD_DEFINITIONS(-DHAVE_MYSQL)
		set(SOURCES ${SOURCES} db/db_mysql.c)
		set(optionalLibs mysqlclient)
	ENDIF(MYSQL_FOUND)
ENDIF(WITH_MYSQL)


add_executable(arpwatch-v6 ${SOURCES})

target_link_libraries(arpwatch-v6 confuse pcap mysqlclient)
