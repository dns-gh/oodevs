// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Events_h_
#define __Events_h_

#include "Values.h"
#include "FireComponentDamages.h"
#include "FireHumanDamages.h"
#include "CrowdHumanDamages.h"

// =============================================================================
/** @namespace  Existences
    @brief      Model values linked to fire/reports/whatever events
*/
// Created: AGE 2007-10-02
// =============================================================================
namespace events
{
    template< typename Extractor >
    struct TickPeriod : public TickValue< typename Extractor::Type >
    {
        enum { has_parameter = Extractor::has_parameter };

        explicit TickPeriod()
            : extractor_() {}
        explicit TickPeriod( xml::xistream& xis )
            : extractor_( xis ) {}
        void Receive( const sword::SimToClient& message )
        {
            if( extractor_.HasValue( message ) )
                Set( extractor_.Extract( message ) );
        }
        Extractor extractor_;
    };

typedef TickPeriod< extractors::FireComponentDamages > FireComponentDamages;
typedef TickPeriod< extractors::FireHumanDamages >     FireHumanDamages;
typedef TickPeriod< extractors::CrowdHumanDamages >    CrowdHumanDamages;
typedef TickPeriod< extractors::DamageIndirectFire >   DamageIndirectFire;
}

#endif // __Events_h_
