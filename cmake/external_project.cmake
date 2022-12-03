include_guard(GLOBAL)
include(ExternalProject)

# ---------------------------------------------------------------------
# --- Define external (imported) project
# ---------------------------------------------------------------------
# Mandatory arguments:
#   1 : project_name    - Name of project
#   2 : project_type    - Type of project (STATIC / SHARED / INTERFACE)
#   3 : project_scope   - Project definition scope (PRIVATE / GLOBAL)
#
# Additional arguments:
#   OBJECT              - Main file path to check it's existence before build (is linked if library)
#   DEPENDENCIES        - List of project dependencies
#   INCLUDE_DIRECTORIES - List of include directories provided by project
#   LINK_LIBRARIES      - List of link libraries and linker flags provided by project
#   SILENT              - Do not print project information
#
function (DF_ExternalProject project_name project_type project_scope)
  cmake_parse_arguments(
    PARSE_ARGV 3 DF_ExternalProject "" "OBJECT;SILENT" "DEPENDENCIES;INCLUDE_DIRECTORIES;LINK_LIBRARIES"
  )

  if (NOT DF_ExternalProject_SILENT)
    message (STATUS "---- External project ----" )
    message (STATUS "  Name         : ${project_name} (${project_type} / ${project_scope})")
  endif ()

  # - Define imported library
  add_library(${project_name} ${project_type} IMPORTED ${project_scope})

  # - Set import location object
  if (DF_ExternalProject_OBJECT)
    if (NOT DF_ExternalProject_SILENT)
      message (STATUS "  Object       : ${DF_ExternalProject_OBJECT}")
    endif ()
    set_property(TARGET ${project_name}
      PROPERTY
        IMPORTED_LOCATION
          "${DF_ExternalProject_OBJECT}"
    )
  endif ()

  # - Set include directories
  if (DF_ExternalProject_INCLUDE_DIRECTORIES)
    message (STATUS "  Include      : ${DF_ExternalProject_INCLUDE_DIRECTORIES}")

    # - Create a phony include dir to avoid cmake errors (will be overriden later)
    execute_process(
      COMMAND mkdir -p ${DF_ExternalProject_INCLUDE_DIRECTORIES}
    )

    set_property(TARGET ${project_name}
      PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES
          ${DF_ExternalProject_INCLUDE_DIRECTORIES}
    )
  endif ()

  # - Set link libraries
  if (DF_ExternalProject_LINK_LIBRARIES)
    message (STATUS "  Libraries    : ${DF_ExternalProject_LINK_LIBRARIES}")

    set_property(TARGET ${project_name}
      PROPERTY
        INTERFACE_LINK_LIBRARIES
          ${DF_ExternalProject_LINK_LIBRARIES}
    )
  endif ()

  # - Set dependencies
  if (DF_ExternalProject_DEPENDENCIES)
    message (STATUS "  Dependencies : ${DF_ExternalProject_DEPENDENCIES}")
    add_dependencies(${project_name}
      ${DF_ExternalProject_DEPENDENCIES}
    )
  endif()

  message (STATUS "--------------------------" )
endfunction()