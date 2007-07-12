// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MessageFilter_h_
#define __MessageFilter_h_

#include "Filter.h"
#include "ValueExtractor_ABC.h"

// =============================================================================
/** @class  MessageFilter
    @brief  MessageFilter
*/
// Created: AGE 2007-07-12
// =============================================================================
class MessageFilter : public Filter
{

public:
    //! @name Constructors/Destructor
    //@{
             MessageFilter( MessageHandler_ABC& handler, const ValueHolder_ABC& value, MessageHandler_ABC& forward )
                 : Filter( value, forward )
                 , filter_( handler ) {}
             MessageFilter( ValueExtractor_ABC& extractor, MessageHandler_ABC& forward )
                 : Filter( extractor, forward )
                 , filter_( extractor ) {}
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        filter_.Receive( message );
        Filter::Receive( message );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MessageFilter( const MessageFilter& );            //!< Copy constructor
    MessageFilter& operator=( const MessageFilter& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    MessageHandler_ABC& filter_;
    //@}
};

#endif // __MessageFilter_h_
