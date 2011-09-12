#include "mitkLookupTable.h"
#include "mitkCSwigMacros.h"

#ifdef CABLE_CONFIGURATION

namespace _cable_
{
     const char* const group="LookupTable";
     namespace wrappers
     {
         MITK_WRAP_OBJECT(LookupTable)
     }
}

#endif

