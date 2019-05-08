project(test-controller-lights)

add_executable(test-controller-lights main.c controller-lights.c)

target_link_libraries(test-controller-lights)

INSTALL(TARGETS test-controller-lights DESTINATION /usr/share/railwayman/tests/bin)
