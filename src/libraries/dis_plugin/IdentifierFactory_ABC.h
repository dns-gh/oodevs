// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __IdentifierFactory_ABC_h_
#define __IdentifierFactory_ABC_h_

#include "rpr/EntityIdentifier.h"

namespace plugins
{
namespace dis
{

// =============================================================================
/** @class  IdentifierFactory_ABC
    @brief  Identifier factory definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class IdentifierFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             IdentifierFactory_ABC() {}
    virtual ~IdentifierFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual rpr::EntityIdentifier CreateNewIdentifier() = 0;
    //@}
};

}
}

#endif // __IdentifierFactory_ABC_h_
