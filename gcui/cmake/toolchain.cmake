set(CMAKE_SYSTEM_NAME               gigachad)
set(CMAKE_SYSTEM_PROCESSOR          arm)

# Without that flag CMake is not able to pass test compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE   STATIC_LIBRARY)

set(CMAKE_AR                        ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-ar${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_ASM_COMPILER              ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_C_COMPILER                ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-gcc${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_CXX_COMPILER              ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-g++${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_LINKER                    ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-ld${CMAKE_EXECUTABLE_SUFFIX})
set(CMAKE_OBJCOPY                   ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-objcopy${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_RANLIB                    ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-ranlib${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_SIZE                      ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-size${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")
set(CMAKE_STRIP                     ${TOOLCHAIN_PATH}arm-buildroot-linux-musleabihf-strip${CMAKE_EXECUTABLE_SUFFIX} CACHE INTERNAL "")

#set(CMAKE_C_FLAGS                   "-Wno-psabi  -fdata-sections -ffunction-sections -Wl,--gc-sections" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS                 "${CMAKE_C_FLAGS} -fno-exceptions" CACHE INTERNAL "")

set(CMAKE_C_FLAGS_DEBUG             "-Os -g" CACHE INTERNAL "")
set(CMAKE_C_FLAGS_RELEASE           "-Os -DNDEBUG" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_C_FLAGS_DEBUG}" CACHE INTERNAL "")
set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_C_FLAGS_RELEASE}" CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)