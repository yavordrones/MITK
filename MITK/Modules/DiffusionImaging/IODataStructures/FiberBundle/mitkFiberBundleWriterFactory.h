/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-05-13 18:06:46 +0200 (Mi, 13 Mai 2009) $
Version:   $Revision: 11215 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef FIBERBUNDLE_WRITERFACTORY_H_HEADER_INCLUDED
#define FIBERBUNDLE_WRITERFACTORY_H_HEADER_INCLUDED

#include "itkObjectFactoryBase.h"
#include "mitkBaseData.h"
#include "MitkDiffusionImagingExports.h"

namespace mitk
{

class MitkDiffusionImaging_EXPORT FiberBundleWriterFactory : public itk::ObjectFactoryBase
{
public:

  mitkClassMacro( mitk::FiberBundleWriterFactory, itk::ObjectFactoryBase )

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    static bool IsRegistered = false;
    if ( !IsRegistered )
    {
      FiberBundleWriterFactory::Pointer ugVtkWriterFactory = FiberBundleWriterFactory::New();
      ObjectFactoryBase::RegisterFactory( ugVtkWriterFactory );
      IsRegistered = true;
    }
  }

protected:
  FiberBundleWriterFactory();
  ~FiberBundleWriterFactory();

private:
  FiberBundleWriterFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace mitk

#endif // FIBERBUNDLE_WRITERFACTORY_H_HEADER_INCLUDED



