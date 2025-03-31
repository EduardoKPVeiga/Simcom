file(GLOB_RECURSE MAIN_SOURCES
   "main.cpp"
   "source/*.cpp"
   "source/Mqtt/*.cpp"
   "source/Mqtt/Header/*.cpp"
)

idf_component_register(
    SRCS ${MAIN_SOURCES}
    INCLUDE_DIRS "." "include" "include/Mqtt" "include/Mqtt/Header"
)