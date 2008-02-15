// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Updatable_ABC_h_
#define __Updatable_ABC_h_

namespace dispatcher
{

// =============================================================================
/** @class  Updatable_ABC
    @brief  Updatable extension base class
*/
// Created: AGE 2006-02-06
// =============================================================================
template< typename T >
class Updatable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Updatable_ABC() {};
    virtual ~Updatable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const T& message ) = 0;
    //@}
};

}

#endif // __Updatable_ABC_h_
