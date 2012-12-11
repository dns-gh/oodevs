// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Fires_h_
#define __Fires_h_

#include "Values.h"
#include "Extractors.h"

// =============================================================================
/** @namespace  fires
    @brief      Model values linked to fires events
*/
// Created: AGE 2007-10-02
// =============================================================================
namespace fires
{
    template< typename Extractor >
    struct FiresBase : public InstantValue< typename Extractor::Type >
    {
        enum { has_parameter = Extractor::has_parameter };

        explicit FiresBase()
            : extractor_() {}
        explicit FiresBase( xml::xistream& xis )
            : extractor_( xis ) {}
        void Receive( const sword::SimToClient& message )
        {
            if( extractor_.HasValue( message ) )
                Set( extractor_.Extract( message ) );
        }
        Extractor extractor_;
    };

typedef FiresBase< extractors::DirectFireTargetsId >          DirectFireTargetsId;
typedef FiresBase< extractors::IndirectFireTargetsPositions > IndirectFireTargetsPositions;
typedef FiresBase< extractors::DirectFireUnitsId >            DirectFireUnitsId;
typedef FiresBase< extractors::IndirectFireUnitsId >          IndirectFireUnitsId;
}

#endif // __Fires_h_
