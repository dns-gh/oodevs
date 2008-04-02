// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PlatformVisitor_ABC_h_
#define __PlatformVisitor_ABC_h_

namespace tic
{
    class Platform_ABC;

// =============================================================================
/** @class  PlatformVisitor_ABC
    @brief  Platform visitor definition
*/
// Created: AGE 2008-04-01
// =============================================================================
class PlatformVisitor_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PlatformVisitor_ABC() {};
    virtual ~PlatformVisitor_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void AddPlatform( const Platform_ABC& platform ) = 0;
    //@}
};

}

#endif // __PlatformVisitor_ABC_h_
