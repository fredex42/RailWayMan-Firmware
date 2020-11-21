#NOTE: need declare -x PKG_CONFIG_PATH=/opt/rpi-rootfs/usr/lib/arm-linux-gnueabihf/pkgconfig:/opt/rpi-rootfs/usr/lib/pkgconfig:/opt/rpi-rootfs/usr/share/pkgconfig to find libs
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER /opt/tools/bin/armv6-rpi-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /opt/tools/bin/armv6-rpi-linux-gnueabihf-g++)
SET(CMAKE_FIND_ROOT_PATH /opt/rpi-rootfs)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH} -L/usr/lib/arm-linux-gnueabihf -L/lib/arm-linux-gnueabihf -Wl,-rpath-link,/usr/lib/arm-linux-gnueabihf -Wl,-rpath-link,/lib/arm-linux-gnueabihf")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} --sysroot=${CMAKE_FIND_ROOT_PATH}")

#tell dpkg what arch we are
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE armhf)

#extra libraries that are link dependencies when cross-compiling
SET(RPI_EXTRA_LIBS icui18n icuuc icudata blkid thai selinux png12 bsd datrie systemd lzma lz4 gpg-error gcrypt ffi mount uuid)

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CROSS_SYSTEM_INCLUDES /opt/rpi-rootfs/usr/include/arm-linux-gnueabihf /opt/rpi-rootfs/usr/include)
