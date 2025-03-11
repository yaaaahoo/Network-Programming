add_library(common SHARED)

target_sources(common
PRIVATE
common.cpp
)

install(TARGETS common DESTINATION lib)