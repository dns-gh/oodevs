// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ExtensionFactory_ABC_h_
#define __ExtensionFactory_ABC_h_

namespace dispatcher
{

// =============================================================================
/** @class  ExtensionFactory_ABC
    @brief  ExtensionFactory_ABC
*/
// Created: SBO 2008-02-13
// =============================================================================
template< typename Entity >
class ExtensionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExtensionFactory_ABC() {}
    virtual ~ExtensionFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Create( Entity& entity ) = 0;
    //@}
};

}

#endif // __ExtensionFactory_ABC_h_
