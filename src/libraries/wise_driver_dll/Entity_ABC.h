// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

#pragma warning( push )
#pragma warning( disable: 4201 )
#include <wise/wiseinterfacetypes.h>
#pragma warning( pop )

class CWISEDriver;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity_ABC
*/
// Created: SEB 2010-10-13
// =============================================================================
class Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC() {}
    virtual ~Entity_ABC() {}
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const = 0;
    virtual WISE_HANDLE GetHandle() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const = 0;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const = 0;
    //@}
};

#endif // __Entity_ABC_h_
