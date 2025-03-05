file(GLOB_RECURSE MAIN_SOURCES
   "main.cpp"
   "source/*.cpp"
)

idf_component_register(
    SRCS ${MAIN_SOURCES}
    INCLUDE_DIRS "." "include"
)