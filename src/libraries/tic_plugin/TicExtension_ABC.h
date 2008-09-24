// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TicExtension_ABC_h_
#define __TicExtension_ABC_h_

#include "clients_kernel/Extension_ABC.h"

namespace plugins
{
namespace tic
{
    class PlatformVisitor_ABC;

// =============================================================================
/** @class  TicExtension_ABC
    @brief  Tic extension definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class TicExtension_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             TicExtension_ABC() {};
    virtual ~TicExtension_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Accept( PlatformVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // __TicExtension_ABC_h_
