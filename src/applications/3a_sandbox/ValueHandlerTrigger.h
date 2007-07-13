// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueHandlerTrigger_h_
#define __ValueHandlerTrigger_h_

#include "ValueHolder_ABC.h"
#include "ValueHandler_ABC.h"
#include "dispatcher/MessageHandler_ABC.h"

// =============================================================================
/** @class  ValueHandlerTrigger
    @brief  ValueHandlerTrigger
*/
// Created: AGE 2007-07-12
// =============================================================================
class ValueHandlerTrigger : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ValueHandlerTrigger( ValueHolder_ABC& holder, ValueHandler_ABC& handler )
                 : holder_( holder )
                 , handler_( handler ) {}
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& )
    {
        handler_.Handle( holder_.GetValue() );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ValueHandlerTrigger( const ValueHandlerTrigger& );            //!< Copy constructor
    ValueHandlerTrigger& operator=( const ValueHandlerTrigger& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ValueHolder_ABC& holder_;
    ValueHandler_ABC& handler_;
    //@}
};

#endif // __ValueHandlerTrigger_h_
