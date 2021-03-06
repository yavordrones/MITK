FIND_PACKAGE(ITK)
IF(ITK_GDCM_DIR)
  INCLUDE(${ITK_GDCM_DIR}/GDCMConfig.cmake)
  IF(GDCM_MAJOR_VERSION EQUAL 2)
    ADD_DEFINITIONS(-DGDCM2)
    SET(ITK_USES_GDCM2 1)
  ENDIF(GDCM_MAJOR_VERSION EQUAL 2)
ENDIF(ITK_GDCM_DIR)

MITK_CREATE_MODULE( MitkDiffusionImaging
  SUBPROJECTS MITK-DTI
  INCLUDE_DIRS IODataStructures Reconstruction Tractography Rendering Algorithms DicomImport Interactions IODataStructures/DiffusionWeightedImages IODataStructures/QBallImages IODataStructures/TensorImages IODataStructures/FiberBundle IODataStructures/PlanarFigureComposite IODataStructures/TbssImages ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS MitkExt SceneSerializationBase QmitkExt
  PACKAGE_DEPENDS Boost
)

MITK_USE_MODULE(MitkDiffusionImaging)
if(MitkDiffusionImaging_IS_ENABLED)
  file(DOWNLOAD http://mitk.org/download/data/FibertrackingLUT.tar.gz ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/FibertrackingLUT.tar.gz TIMEOUT 10)
  execute_process(COMMAND cmake -E chdir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} tar xzf FibertrackingLUT.tar.gz)
endif()

ADD_SUBDIRECTORY(Testing)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/mitkDiffusionImagingConfigure.h.in ${CMAKE_CURRENT_BINARY_DIR}/mitkDiffusionImagingConfigure.h)

