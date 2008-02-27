// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntitySymbols_ABC_h_
#define __EntitySymbols_ABC_h_

#include "clients_kernel/Extension_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  EntitySymbols_ABC
    @brief  EntitySymbols_ABC
*/
// Created: SBO 2008-02-13
// =============================================================================
class EntitySymbols_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EntitySymbols_ABC() {}
    virtual ~EntitySymbols_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const = 0;
    //@}
};

}

#endif // __EntitySymbols_ABC_h_
