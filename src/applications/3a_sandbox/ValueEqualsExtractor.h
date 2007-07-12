// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueEqualsExtractor_h_
#define __ValueEqualsExtractor_h_

#include "MessageHandler_ABC.h"
#include "ValueEquals.h"

// =============================================================================
/** @class  ValueEqualsExtractor
    @brief  ValueEqualsExtractor
*/
// Created: AGE 2007-07-12
// =============================================================================
template< typename Extractor >
class ValueEqualsExtractor : public ValueEquals, public MessageHandler_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ValueEqualsExtractor( float value )
        : ValueEquals( extractor_, value ) {}
    //@}

    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message )
    {
        extractor_.Receive( message );
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ValueEqualsExtractor( const ValueEqualsExtractor& );            //!< Copy constructor
    ValueEqualsExtractor& operator=( const ValueEqualsExtractor& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Extractor extractor_;
    //@}
};

#endif // __ValueEqualsExtractor_h_
