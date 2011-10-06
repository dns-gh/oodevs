// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Existences_h_
#define __Existences_h_

#include "Values.h"
#include "Extractors.h"
#include "Carriers.h"

// =============================================================================
/** @namespace  Existences
    @brief      Model values linked to unit/object/population/log existence
*/
// Created: AGE 2007-09-13
// =============================================================================
namespace existences
{
    template< typename Extractor >
    struct Existence : public PulsedValue< typename Extractor::Type >
    {
        enum { has_parameter = Extractor::has_parameter };

        explicit Existence( const Extractor& extractor = Extractor() )
            : extractor_( extractor ) {}
        explicit Existence( xml::xistream& xis )
            : extractor_( xis ) {}
        void Receive( const sword::SimToClient& message )
        {
            if( extractor_.IsCreation( message ) )
                Set( extractor_.Extract( message ) );
            else if( extractor_.IsDestruction( message ) )
                Reset();
        }
        Extractor extractor_;
    };

typedef Existence< extractors::MaintenanceHandlingUnitId >    MaintenanceHandlingUnitId;
typedef Existence< extractors::DirectFireTargetsId >          DirectFireTargetsId;
typedef Existence< extractors::IndirectFireTargetsPositions > IndirectFireTargetsPositions;
typedef Existence< extractors::DirectFireUnitsId >            DirectFireUnitsId;
typedef Existence< extractors::IndirectFireUnitsId >          IndirectFireUnitsId;
typedef Existence< extractors::Carriers >                     Carriers;
}

#endif // __Existences_h_
