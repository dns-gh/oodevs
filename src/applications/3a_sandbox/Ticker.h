// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Ticker_h_
#define __Ticker_h_

#include "dispatcher/MessageHandler_ABC.h"
#include "MessageFilter.h"
#include "ValueEqualsExtractor.h"
#include "ValueHandlerTrigger.h"
#include "TypeExtractor.h"

// =============================================================================
/** @class  Ticker
    @brief  Ticker
*/
// Created: AGE 2007-07-12
// =============================================================================
class Ticker : public dispatcher::MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Ticker( ValueHolder_ABC& holder, ValueHandler_ABC& handler )
                 : trigger_( holder, handler )
                 , equals_( float( T_MsgsSimToClient_msg_msg_control_end_tick ) )
                 , filter_( equals_, equals_, trigger_ ) {}
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        filter_.Receive( message );
    };
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Ticker( const Ticker& );            //!< Copy constructor
    Ticker& operator=( const Ticker& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ValueHandlerTrigger                   trigger_;
    ValueEqualsExtractor< TypeExtractor > equals_;
    MessageFilter                         filter_;
    //@}
};

#endif // __Ticker_h_
