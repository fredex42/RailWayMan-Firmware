project(test-controller)

link_directories(/opt/rpi-rootfs/lib/arm-linux-gnueabihf /opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf )

add_executable(test-tracksection test-tracksection.c tracksection-functions.c ../controller-common/i2c.c)

#no idea why i have to put the -rpath-link option in here too, must be something to do with commandline ordering, but at least it works.
target_link_libraries(test-tracksection "-Wl,-rpath-link,/opt/rpi-rootfs/lib/arm-linux-gnueabihf:/opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf" ncurses)
target_include_directories(test-tracksection PUBLIC ${CROSS_SYSTEM_INCLUDES})

INSTALL(TARGETS test-tracksection DESTINATION /usr/share/railwayman/tests/bin)

add_executable(test-convert-signed-speed test-convert-signed-speed.c tracksection-functions.c ../controller-common/i2c.c)
set_target_properties(test-convert-signed-speed PROPERTIES LINK_SEARCH_START_STATIC 1)
set_target_properties(test-convert-signed-speed PROPERTIES LINK_SEARCH_END_STATIC 1)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_test(test-convert-signed-speed test-convert-signed-speed)
