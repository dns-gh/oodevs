// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Value_ABC_h_
#define __Value_ABC_h_

#include "Displayer_ABC.h"
// =============================================================================
/** @class  Value_ABC
    @brief  Value_ABC
*/
// Created: AGE 2006-06-22
// =============================================================================
class Value_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Value_ABC() {};
    virtual ~Value_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void Display( Displayer_ABC& displayer ) const = 0;
    //@}
};

template< typename T >
class Value : public Value_ABC
{
public:
    explicit Value( const T& value )
        : data_( &value ) {};
    virtual ~Value() {};

    virtual void Display( Displayer_ABC& displayer ) const
    {
        displayer.Display( *data_ );
    }
private:
    const T* data_;
};

#endif // __Value_ABC_h_
