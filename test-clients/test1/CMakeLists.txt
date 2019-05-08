project(test1)

add_executable(test1 test1.c)

target_link_libraries(test1)

INSTALL(TARGETS test1 DESTINATION /usr/share/railwayman/tests/bin)
