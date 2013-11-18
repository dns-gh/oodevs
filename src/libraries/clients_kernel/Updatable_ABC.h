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

namespace kernel
{

// =============================================================================
/** @class  Updatable_ABC
    @brief  Updatable definition
*/
// Created: AGE 2006-02-07
// =============================================================================
template< typename T >
class Updatable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Updatable_ABC() {}
    virtual ~Updatable_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const T& /*updateMessage*/ ) {};
    virtual void DoUpdateContext( const T& /*updateMessage*/, int /*context*/ ) {};
    //@}
};

}

#endif // __Updatable_ABC_h_
