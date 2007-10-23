// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Events_h_
#define __Events_h_

#include "Values.h"
#include "Extractors.h"

// =============================================================================
/** @namespace  Existences
    @brief      Model values linked to fire/reports/whatever events
*/
// Created: AGE 2007-10-02
// =============================================================================
namespace events
{
    template< typename Extractor >
    struct Event : public InstantValue< typename Extractor::Type >
    {
        enum { has_parameter = Extractor::has_parameter };

        explicit Event( const Extractor& extractor = Extractor() )
            : extractor_( extractor ) {}
        void Receive( const ASN1T_MsgsSimToClient& message )
        {
            if( extractor_.HasValue( message ) )
                Set( extractor_.Extract( message ) );
        }
        Extractor extractor_;
    };

typedef Event< extractors::FireComponentDamage > FireComponentDamage;
}

#endif // __Events_h_
