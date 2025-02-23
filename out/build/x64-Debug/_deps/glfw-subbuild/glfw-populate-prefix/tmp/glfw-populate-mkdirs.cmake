# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-src")
  file(MAKE_DIRECTORY "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-build"
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix"
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/tmp"
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp"
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/src"
  "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/karen/source/repos/year3diss/out/build/x64-Debug/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
