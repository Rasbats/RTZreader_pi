cmake_minimum_required(VERSION 3.1)

if (TARGET ocpn::xercesc)
    return ()
endif ()

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

if (WIN32)
  file(GLOB libxerces_dir "${CMAKE_CURRENT_SOURCE_DIR}/xercesc")
  add_library(XERCES STATIC IMPORTED)
  set_property(
    TARGET XERCES
    PROPERTY IMPORTED_LOCATION ${libxerces_dir}/lib/xerces-c_3.lib
  )
  add_library(_XERCES INTERFACE)
  target_link_libraries(_XERCES INTERFACE XERCES)
  target_include_directories(_XERCES INTERFACE ${libxerces_dir}/include)
  add_library(ocpn::xercesc ALIAS _XERCES)
else ()
  include(FindXercesC)
  find_package(XercesC REQUIRED)
  add_library(_XercesC INTERFACE)
  target_include_directories(_XercesC INTERFACE ${XercesC_INCLUDE_DIR})
  target_link_libraries(_XercesC INTERFACE ${XercesC_LIBRARY})
  add_library(ocpn::xercesc ALIAS _XercesC)
endif ()
