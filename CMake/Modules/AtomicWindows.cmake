
set (CMAKE_DEBUG_POSTFIX _d)

set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
)

foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

add_definitions (-D_CRT_SECURE_NO_WARNINGS)
set (CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELEASE} /fp:fast /Zi /GS-")
set (CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELWITHDEBINFO})
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /fp:fast /Zi /GS- /D _SECURE_SCL=0")
set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})
# SSE flag is redundant if already compiling as 64bit
if (ATOMIC_SSE AND NOT ATOMIC_64BIT)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE")
endif ()
set (CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /DEBUG")
set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF")

list (APPEND ATOMIC_LINK_LIBRARIES MojoShader user32 gdi32 winmm imm32 ole32 oleaut32 version uuid d3d9 d3dcompiler)