cmake_minimum_required(VERSION 3.19)
project(deps)

include(ExternalProject)

ExternalProject_Add(gflags
    GIT_REPOSITORY https://github.com/gflags/gflags.git
    GIT_TAG f8a0efe03aa69b3336d8e228b37d4ccb17324b88 # v2.2.0

    CMAKE_ARGS
    # the next two are standard and needed for all deps
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/install/gflags

    -DREGISTER_INSTALL_PREFIX=FALSE
    -DGFLAGS_BUILD_STATIC_LIBS=TRUE
    -DGFLAGS_BUILD_SHARED_LIBS=FALSE
    -DGFLAGS_BUILD_TESTING=FALSE    
)

ExternalProject_add(glog
    GIT_REPOSITORY https://github.com/google/glog.git
    GIT_TAG 96a2f23dca4cc7180821ca5f32e526314395d26a # v0.4.0

    CMAKE_ARGS
    # the next two are standard and needed for all deps
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_PREFIX_PATH:PATH=${CMAKE_BINARY_DIR}/install/gflags
    -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/install/glog

    -DBUILD_TESTING=FALSE

    DEPENDS gflags)

# Fetch googletest
ExternalProject_add(
  GTest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG e2239ee6043f73722e7aa812a459f54a28552929 # v1.11.0

  CMAKE_ARGS
  # the next two are standard and needed for all deps
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/install/GTest
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)