set(COMPONENT_ADD_INCLUDEDIRS
    src
    )

set(COMPONENT_PRIV_REQUIRES)
file(GLOB SRCS src/servoControl.cpp)

set(COMPONENT_REQUIRES)
set(COMPONENT_SRCS ${SRCS})

register_component()

