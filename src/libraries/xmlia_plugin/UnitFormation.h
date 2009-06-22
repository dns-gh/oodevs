// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UniteFormation_h_
#define __UniteFormation_h_

#include "Unit_ABC.h"

namespace dispatcher
{
  class Formation;
}

namespace xml
{
  class xistream;
  class xostream;
}

namespace plugins
{
  namespace xmlia
  {

    // =============================================================================
    /** @class  UniteFormation
    @brief  UniteFormation
    */
    // Created: MGD 2009-06-12
    // =============================================================================
    class UnitFormation : public Unit_ABC
    {

    public:
      //! @name Constructors/Destructor
      //@{
      UnitFormation( dispatcher::Formation& formation );
      UnitFormation( xml::xistream& xis );
      virtual ~UnitFormation();
      //@}
    };

  }// xmlia

}//plugin

#endif // __UniteFormation_h_
