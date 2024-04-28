

    set(xercesc_repo https://github.com/apache/xerces-c.git)
    # set(mnav_tag v0.14.0)   ## Does not build on gcc12/Bookworm
    set(xerces_tag 5fe4f4b)

    add_library(XERCES INTERFACE)
    add_library(ocpn::xercesc ALIAS XERCES)

    if (EXISTS /app/bin)
      # Flatpak build, marnav built as a extension module
      #
      add_library(_xercesc_lib STATIC IMPORTED)
      file(GLOB _xercesc_path /app/extensions/*/lib/libxerces-c.a)
      set_property(TARGET _xercesc_lib PROPERTY IMPORTED_LOCATION ${_xercesc_path})

      add_library(_xercesc_io_lib STATIC IMPORTED)
      file(GLOB _xercesc_io_path /app/extensions/*/lib/libxerces-c-io.a)
      set_property(TARGET _xercesc_io_lib PROPERTY IMPORTED_LOCATION ${_xercesc_io_path})

      target_link_libraries(XERCES INTERFACE _xercesc_lib _xercesc_io_lib)

      string(TOLOWER  ${PACKAGE_NAME} _lc_pkg_name)
      set(xercesc_incl_path /app/extensions/${_lc_pkg_name}/include)
      target_include_directories(XERCES INTERFACE  ${xercesc_incl_path})
    else ()
      # Download sources and get the source directory
      #
      include(FetchContent)
      FetchContent_Declare(xercesc GIT_REPOSITORY ${xercesc_repo} GIT_TAG ${xercesc_tag})
      FetchContent_Populate(xercesc)
      FetchContent_GetProperties(xercesc SOURCE_DIR xercesc_src_dir)


      # Set up build options and build.
      #
      set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Build shared libs" FORCE)

      add_subdirectory(${xercesc_src_dir} xercesc_bld)

      # Get the include directories
      file(GLOB xercesc_incl_dir "${CMAKE_SOURCE_DIR}/include")
      message(STATUS "Include directory is now: ${CMAKE_SOURCE_DIR}/include")
      target_include_directories(XERCES INTERFACE  ${xercesc_incl_dir})

      #message(STATUS "Directory here: ${xercesc_bld}")
     # get_property(dirs DIRECTORY ${xercesc_bld} PROPERTY INCLUDE_DIRECTORIES)
      #  foreach(dir ${dirs})
      #    message(STATUS "dir='${dir}'")
      #  endforeach()

     # target_include_directories(${xercesc_bld}/include INTERFACE ocpn::xercesc )
      # Link available libraries
      #
      target_link_libraries(XERCES INTERFACE ocpn::xercesc)

    endif ()

