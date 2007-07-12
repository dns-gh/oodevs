// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TypeExtractor_h_
#define __TypeExtractor_h_

#include "SimpleValueExtractor_ABC.h"

// =============================================================================
/** @class  TypeExtractor
    @brief  TypeExtractor
*/
// Created: AGE 2007-07-12
// =============================================================================
class TypeExtractor : public SimpleValueExtractor_ABC
{

protected:
    //! @name Operations
    //@{
    virtual float ExtractValue( const ASN1T_MsgsSimToClient& message )
    {
        return float( message.msg.t );
    }
    //@}
};

#endif // __TypeExtractor_h_
