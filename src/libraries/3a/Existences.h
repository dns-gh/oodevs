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

// =============================================================================
/** @namespace  Existences
    @brief      Model values linked to unit/object/population/log existence
*/
// Created: AGE 2007-09-13
// =============================================================================
namespace existences
{
    template< typename Extractor >
    struct Existence : public ContinuousValue< bool >
    {
        explicit Existence( const Extractor& extractor = Extractor() )
            : extractor_( extractor ) {}
        void Receive( const ASN1T_MsgsSimToClient& message )
        {
            if( extractor_.IsCreation( message ) )
                Set( true );
            else if( extractor_.IsDestruction( message ) )
                Set( false );
        }
        Extractor extractor_;
    };

    struct MaintenanceHandling : public Existence< extractors::MaintenanceHandling > {};
}

#endif // __Existences_h_
