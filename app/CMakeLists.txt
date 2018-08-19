# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp )

include_directories( include/OGRE )
include_directories( include/OGRE/Android )
include_directories( include/OGRE/Bites )
include_directories( include/OGRE/HLMS )
include_directories( include/OGRE/MeshLodGenerator )
include_directories( include/OGRE/Overlay )
include_directories( include/OGRE/Paging )
include_directories( include/OGRE/Plugins/OctreeSceneManager )
include_directories( include/OGRE/Plugins/ParticleFX )
include_directories( include/OGRE/Property )
include_directories( include/OGRE/RenderSystems/GLES2 )
include_directories( include/OGRE/RTShaderSystem )
include_directories( include/OGRE/Terrain )
include_directories( include/OGRE/Threading )
include_directories( include/OGRE/Volume )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       ${CMAKE_SOURCE_DIR}/lib/libOgreBitesStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/OGRE/libPlugin_OctreeSceneManagerStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/OGRE/libPlugin_ParticleFXStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/libOgreOverlayStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/libOgreRTShaderSystemStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/libOgreHLMSStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/OGRE/libRenderSystem_GLES2Static.a
                       ${CMAKE_SOURCE_DIR}/lib/libOgreGLSupportStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/libOgreMainStatic.a
                       ${CMAKE_SOURCE_DIR}/lib/libFreeImage.a
                       ${CMAKE_SOURCE_DIR}/lib/libfreetype.a
                       ${CMAKE_SOURCE_DIR}/lib/libzziplib.a
                       ${CMAKE_SOURCE_DIR}/lib/libcpufeatures.a

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )