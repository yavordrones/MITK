/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-05-26 17:32:07 +0200 (Tue, 26 May 2009) $
Version:   $Revision: 17418 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKGPUVOLUMEMAPPER3D_H_HEADER_INCLUDED
#define MITKGPUVOLUMEMAPPER3D_H_HEADER_INCLUDED

#include "mitkCommon.h"
#include "MitkExtExports.h"

#include "mitkBaseRenderer.h"
#include "mitkVtkMapper3D.h"
#include "mitkImage.h"

#include <vtkFixedPointVolumeRayCastMapper.h>
#include "vtkMitkVolumeTextureMapper3D.h"

// Only with VTK 5.6 or above
#if ((VTK_MAJOR_VERSION > 5) || ((VTK_MAJOR_VERSION==5) && (VTK_MINOR_VERSION>=6) ))

#include "vtkMitkGPUVolumeRayCastMapper.h"

#endif

#include <vtkVolumeProperty.h>
#include <vtkImageChangeInformation.h>

namespace mitk {

  /************************************************************************/
  /* Properties that influence the mapper are:
  *
  * - \b "level window": for the level window of the volume data
  * - \b "LookupTable" : for the lookup table of the volume data
  * - \b "TransferFunction" (mitk::TransferFunctionProperty): for the used transfer function of the volume data
  ************************************************************************/

//##Documentation
//## @brief Vtk-based mapper for VolumeData
//##
//## @ingroup Mapper
class MitkExt_EXPORT GPUVolumeMapper3D : public VtkMapper3D
{
public:
  mitkClassMacro(GPUVolumeMapper3D, VtkMapper3D);

  itkNewMacro(Self);

  virtual const mitk::Image* GetInput();
  
  virtual vtkProp *GetVtkProp(mitk::BaseRenderer *renderer);

  virtual void ApplyProperties(vtkActor* actor, mitk::BaseRenderer* renderer);
  static void SetDefaultProperties(mitk::DataNode* node, mitk::BaseRenderer* renderer = NULL, bool overwrite = false);

  /** Returns true if this Mapper currently allows for Level-of-Detail rendering.
   * This reflects whether this Mapper currently invokes StartEvent, EndEvent, and
   * ProgressEvent on BaseRenderer. */
  virtual bool IsLODEnabled( BaseRenderer *renderer = NULL ) const;
  bool IsMIPEnabled( BaseRenderer *renderer = NULL );
  bool IsGPUEnabled( BaseRenderer *renderer = NULL );
  bool IsRAYEnabled( BaseRenderer *renderer = NULL );
  
  virtual void MitkRenderVolumetricGeometry(mitk::BaseRenderer* renderer);

protected:

  GPUVolumeMapper3D();
  virtual ~GPUVolumeMapper3D();

  bool IsRenderable(mitk::BaseRenderer* renderer);

  void InitCommon();
  void DeinitCommon();

  void InitCPU(mitk::BaseRenderer* renderer);
  void DeinitCPU(mitk::BaseRenderer* renderer);
  void GenerateDataCPU(mitk::BaseRenderer* renderer);

  bool InitGPU(mitk::BaseRenderer* renderer);
  void DeinitGPU(mitk::BaseRenderer* renderer);
  void GenerateDataGPU(mitk::BaseRenderer* renderer);

// Only with VTK 5.6 or above
#if ((VTK_MAJOR_VERSION > 5) || ((VTK_MAJOR_VERSION==5) && (VTK_MINOR_VERSION>=6) ))

  bool InitRAY(mitk::BaseRenderer* renderer);
  void DeinitRAY(mitk::BaseRenderer* renderer);
  void GenerateDataRAY(mitk::BaseRenderer* renderer);

#endif

  void InitVtkMapper(mitk::BaseRenderer* renderer);

  virtual void GenerateDataForRenderer(mitk::BaseRenderer* renderer);

  void CreateDefaultTransferFunctions();
  void UpdateTransferFunctions( mitk::BaseRenderer *renderer );

  vtkVolume * m_VolumeNULL;
  
  bool m_commonInitialized;
  vtkImageChangeInformation* m_UnitSpacingImageFilter;
  vtkPiecewiseFunction *m_DefaultOpacityTransferFunction;
  vtkPiecewiseFunction *m_DefaultGradientTransferFunction;
  vtkColorTransferFunction *m_DefaultColorTransferFunction;
  vtkPiecewiseFunction *m_BinaryOpacityTransferFunction;
  vtkPiecewiseFunction *m_BinaryGradientTransferFunction;
  vtkColorTransferFunction *m_BinaryColorTransferFunction;
  
  class LocalStorage : public mitk::Mapper::BaseLocalStorage
  {
    public:

    bool m_cpuInitialized;
    vtkVolume *m_VolumeCPU;
    vtkFixedPointVolumeRayCastMapper* m_MapperCPU;
    vtkVolumeProperty* m_VolumePropertyCPU;

    bool m_gpuSupported;
    bool m_gpuInitialized;
    vtkVolume *m_VolumeGPU;
    vtkMitkVolumeTextureMapper3D* m_MapperGPU;
    vtkVolumeProperty* m_VolumePropertyGPU;

// Only with VTK 5.6 or above
#if ((VTK_MAJOR_VERSION > 5) || ((VTK_MAJOR_VERSION==5) && (VTK_MINOR_VERSION>=6) ))

    bool m_raySupported;
    bool m_rayInitialized;
    vtkVolume *m_VolumeRAY;
    vtkMitkGPUVolumeRayCastMapper* m_MapperRAY;
    vtkVolumeProperty* m_VolumePropertyRAY;

#endif

    LocalStorage()
    {
      m_cpuInitialized = false;
      
      m_gpuInitialized = false;
      m_gpuSupported = true;    // assume initially gpu slicing is supported

// Only with VTK 5.6 or above
#if ((VTK_MAJOR_VERSION > 5) || ((VTK_MAJOR_VERSION==5) && (VTK_MINOR_VERSION>=6) ))
      m_rayInitialized = false;
      m_raySupported = true;    // assume initially gpu raycasting is supported
#endif
    }
    
    ~LocalStorage()
    {
      if(m_cpuInitialized)
      {
        m_VolumeCPU->Delete();
        m_MapperCPU->Delete();
        m_VolumePropertyCPU->Delete();
        m_cpuInitialized=false;
      }

      if(m_gpuInitialized)
      {
        m_VolumeGPU->Delete();
        m_MapperGPU->Delete();
        m_VolumePropertyGPU->Delete();
        m_gpuInitialized=false;
      }

// Only with VTK 5.6 or above
#if ((VTK_MAJOR_VERSION > 5) || ((VTK_MAJOR_VERSION==5) && (VTK_MINOR_VERSION>=6) ))

      if(m_rayInitialized)
      {
        m_VolumeRAY->Delete();
        m_MapperRAY->Delete();
        m_VolumePropertyRAY->Delete();
        m_rayInitialized=false;
      }

#endif

    }
  };  
    
  mitk::Mapper::LocalStorageHandler<LocalStorage> m_LSH;  
};

} // namespace mitk

#endif /* MITKVOLUMEDATAVTKMAPPER3D_H_HEADER_INCLUDED */
