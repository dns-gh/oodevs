// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PlatformDelegate_ABC_h_
#define __PlatformDelegate_ABC_h_

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
class PlatformDelegate_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PlatformDelegate_ABC() {}
    virtual ~PlatformDelegate_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Accept( PlatformVisitor_ABC& visitor ) const = 0;
    //@}
};

}
}

#endif // __PlatformDelegate_ABC_h_
