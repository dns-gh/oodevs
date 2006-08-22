// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Resolver_ABC_h_
#define __Resolver_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  Resolver_ABC
    @brief  Resolver definition
*/
// Created: AGE 2006-02-14
// =============================================================================
template< typename T, typename Identifier = unsigned long >
class Resolver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Resolver_ABC() {};
    virtual ~Resolver_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual T* Find( const Identifier& id ) const = 0;
    virtual T& Get ( const Identifier& id ) const = 0;
    //@}
};

}

#endif // __Resolver_ABC_h_
