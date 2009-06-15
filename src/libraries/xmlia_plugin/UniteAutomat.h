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

#include "Unite_ABC.h"

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
  class UniteAutomat : public Unite_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    UniteAutomat( const dispatcher::Automat& automat );
    UniteAutomat( xml::xistream& xis );
    virtual ~UniteAutomat();
    //@}
};

}// xmlia

}//plugin

#endif // __UniteAutomat_h_
