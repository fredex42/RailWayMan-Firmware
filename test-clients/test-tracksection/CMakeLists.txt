project(test-controller)

link_directories(/opt/rpi-rootfs/lib/arm-linux-gnueabihf /opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf )

add_executable(test-tracksection test-tracksection.c tracksection-functions.c ../controller-common/i2c.c)

#no idea why i have to put the -rpath-link option in here too, must be something to do with commandline ordering, but at least it works.
target_link_libraries(test-tracksection "-Wl,-rpath-link,/opt/rpi-rootfs/lib/arm-linux-gnueabihf:/opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf" ncurses)
target_include_directories(test-tracksection PUBLIC ${CROSS_SYSTEM_INCLUDES})

INSTALL(TARGETS test-tracksection DESTINATION /usr/share/railwayman/tests/bin)
#
#add_executable(test-controller-funcs test-controller-funcs.c controller-funcs.c)
#add_test(test-controller-funcs test-controller-funcs)
