// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttributeExtractor_h_
#define __AttributeExtractor_h_

#include "SimpleValueExtractor_ABC.h"

#define MAKE_EXTRACTOR( name, member )                                   \
struct name                                                              \
{                                                                        \
    static bool HasFlag ( const ASN1T_MsgUnitAttributes& attributes )    \
        { return attributes.m.member##Present; }                         \
    static float Extract( const ASN1T_MsgUnitAttributes& attributes )    \
        { return float( attributes.member ); }                           \
};

MAKE_EXTRACTOR( OpState, etat_operationnel_brut );
MAKE_EXTRACTOR( Heading, direction );
MAKE_EXTRACTOR( Speed,   vitesse );

// =============================================================================
/** @class  AttributeExtractor
    @brief  AttributeExtractor
*/
// Created: AGE 2007-07-12
// =============================================================================
template< typename Extractor >
class AttributeExtractor : public SimpleValueExtractor_ABC
{
public:
    //! @name Operations
    //@{
    virtual float ExtractValue( const ASN1T_MsgsSimToClient& message )
    {
        if( message.msg.t == T_MsgsSimToClient_msg_msg_unit_attributes )
        {
            const ASN1T_MsgUnitAttributes& attributes = *message.msg.u.msg_unit_attributes;
            if( Extractor::HasFlag( attributes ) )
                return Extractor::Extract( attributes );
        }
        return Noop();
    }
    //@}
};

#endif // __AttributeExtractor_h_
