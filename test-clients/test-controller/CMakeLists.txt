project(test-controller)

link_directories(/opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf /opt/rpi-rootfs/lib/arm-linux-gnueabihf ${LIBXMLPP_LIBRARY_DIR})

add_executable(test-controller test-controller.c controller-funcs.c)

target_link_libraries(test-controller)

INSTALL(TARGETS test-controller DESTINATION /usr/share/railwayman/tests/bin)

add_executable(test-controller-funcs test-controller-funcs.c controller-funcs.c)
add_test(test-controller-funcs test-controller-funcs)
