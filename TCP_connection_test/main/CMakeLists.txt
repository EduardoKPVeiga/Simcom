# Register the main application and include sources from subdirectories
file(GLOB_RECURSE MAIN_SOURCES
    "main.c"
    "source/*.cpp"
)

idf_component_register(
    SRCS ${MAIN_SOURCES}
    INCLUDE_DIRS "." "include"
)
