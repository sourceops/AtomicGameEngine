
add_subdirectory(ThirdParty)
add_subdirectory(Atomic)
add_subdirectory(AtomicPlayer)

if (NOT IOS AND NOT ANDROID AND NOT EMSCRIPTEN)
add_subdirectory(Tools)

# Development
add_definitions(-DPOCO_NO_AUTOMATIC_LIBS)

include_directories ( ${CMAKE_SOURCE_DIR}/Source/ThirdParty
                      ${CMAKE_SOURCE_DIR}/Source/ThirdParty/rapidjson/include
                      ${CMAKE_SOURCE_DIR}/Source/ThirdParty/nativefiledialog
                      ${CMAKE_SOURCE_DIR}/Source/ThirdParty/Poco/Foundation/include )

add_subdirectory(AtomicDev)
add_subdirectory(AtomicCLI)
add_subdirectory(AtomicEditor)

endif()


