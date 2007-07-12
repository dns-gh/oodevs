// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ValueExtractor_ABC_h_
#define __ValueExtractor_ABC_h_

#include "ValueHolder_ABC.h"
#include "MessageHandler_ABC.h"

// =============================================================================
/** @class  ValueExtractor_ABC
    @brief  ValueExtractor_ABC
*/
// Created: AGE 2007-07-12
// =============================================================================
class ValueExtractor_ABC : public ValueHolder_ABC, public MessageHandler_ABC
{

public:
    //! @name Operations
    //@{
    virtual void Receive( const ASN1T_MsgsSimToClient& message ) = 0;
    virtual float GetValue() const = 0;
    //@}
};

#endif // __ValueExtractor_ABC_h_
