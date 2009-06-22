// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UniteAutomat_h_
#define __UniteAutomat_h_

#include "Unit_ABC.h"

namespace dispatcher
{
  class Automat;
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
/** @class  UniteAutomat
    @brief  UniteAutomat
*/
// Created: MGD 2009-06-12
// =============================================================================
  class UnitAutomate : public Unit_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    UnitAutomate( const dispatcher::Automat& automat );
    UnitAutomate( xml::xistream& xis );
    virtual ~UnitAutomate();
    //@}
};

}// xmlia

}//plugin

#endif // __UniteAutomat_h_
