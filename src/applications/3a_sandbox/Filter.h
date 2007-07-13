// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Filter_h_
#define __Filter_h_

#include "dispatcher/MessageHandler_ABC.h"
#include "ValueHolder_ABC.h"

// =============================================================================
/** @class  Filter
    @brief  Filter
*/
// Created: AGE 2007-07-12
// =============================================================================
class Filter : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Filter( const ValueHolder_ABC& value, MessageHandler_ABC& forward )
                 : value_  ( value )
                 , forward_( forward ) {} 
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        if( value_.GetValue() )
            forward_.Receive( message );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Filter( const Filter& );            //!< Copy constructor
    Filter& operator=( const Filter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const ValueHolder_ABC& value_;
    MessageHandler_ABC& forward_;
    //@}
};

#endif // __Filter_h_
