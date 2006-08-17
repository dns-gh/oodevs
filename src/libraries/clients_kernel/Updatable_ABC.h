// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Updatable_ABC_h_
#define __Updatable_ABC_h_

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
             Updatable_ABC() {};
    virtual ~Updatable_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const T& updateMessage ) = 0; // $$$$ AGE 2006-02-15: DoUpdate c'est pas terrible
    //@}
};

#endif // __Updatable_ABC_h_
