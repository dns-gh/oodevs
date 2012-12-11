// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Damages_h_
#define __Damages_h_

#include "Values.h"
#include "FireComponentDamages.h"
#include "FireHumanDamages.h"
#include "CrowdHumanDamages.h"

// =============================================================================
/** @namespace  damages
    @brief      Model values linked to Damages events
*/
// Created: AGE 2007-10-02
// =============================================================================
namespace damages
{
    template< typename Extractor >
    struct DamagesBase : public PoolValue< typename Extractor::Type >
    {
        enum { has_parameter = Extractor::has_parameter };

        explicit DamagesBase()
            : extractor_() {}
        explicit DamagesBase( xml::xistream& xis )
            : extractor_( xis ) {}
        void Receive( const sword::SimToClient& message )
        {
            if( extractor_.HasValue( message ) )
                Set( extractor_.Extract( message ) );
        }
        Extractor extractor_;
    };

typedef DamagesBase< extractors::FireComponentDamages > FireComponentDamages;
typedef DamagesBase< extractors::FireHumanDamages >     FireHumanDamages;
typedef DamagesBase< extractors::CrowdHumanDamages >    CrowdHumanDamages;
typedef DamagesBase< extractors::DamageIndirectFire >   DamageIndirectFire;
}

#endif // __Damages_h_
