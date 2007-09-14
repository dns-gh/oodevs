// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Attributes_h_
#define __Attributes_h_

#include "Values.h"
#include "Extractors.h"

// =============================================================================
/** @namespace  Attributes
    @brief      Attributes
*/
// Created: AGE 2007-08-28
// =============================================================================

namespace attributes
{

// =============================================================================
/** @class  UnitAttribute
    @brief  UnitAttribute function
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename Extractor >
struct UnitAttribute : public ContinuousValue< typename Extractor::Type >
{
    explicit UnitAttribute( const Extractor& extractor = Extractor() )
        : extractor_( extractor ) {}

    void Receive( const ASN1T_MsgsSimToClient& message )
    {
        if( message.msg.t == T_MsgsSimToClient_msg_msg_unit_attributes )
        {
            const ASN1T_MsgUnitAttributes& attributes = *message.msg.u.msg_unit_attributes;
            if( extractor_.HasFlag( attributes ) )
                Set( extractor_.Extract( attributes ) );
        }
    }

    Extractor extractor_;
};

struct OperationalState : public UnitAttribute< extractors::OperationalState > {};
struct Position         : public UnitAttribute< extractors::Position > {};

} // namespace attributes


#endif // __Attributes_h_
