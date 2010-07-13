// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "UnitDetection.h"
#include "protocol/Simulation.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: UnitDetection constructor
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
UnitDetection::UnitDetection()
{
    // NOTHING
}

namespace
{
    const char* visibility[5] =
    {
        "invisible",
        "detected",
        "recognized",
        "identified",
        "recorded",
    };
    int ReadMask( xml::xistream& xis, const std::string& attribute )
    {
        if( xis.attribute< std::string >( attribute ).empty() )
            return 1;
        FilterHelper< std::string > states( xis, attribute );
        int result = 0;
        for( unsigned i = 0; i < 5; ++i )
            if( states.IsAllowed( visibility[i] ) )
                result |= ( 1 << i );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: UnitDetection constructor
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
UnitDetection::UnitDetection( xml::xistream& xis )
    : detectedUnitId_( xis.attribute< unsigned long >( "detected" ) )
    , visibilityMask_( ReadMask( xis, "visibility" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitDetection::Extract
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
float UnitDetection::Extract( const MsgSimToClient& wrapper ) const
{
    const MsgsSimToClient::MsgUnitDetection& message = wrapper.message().unit_detection();
    if( detectedUnitId_ == unsigned long( message.detected_unit_oid() ) && ( visibilityMask_ & ( 1 << message.current_visibility() ) ) != 0 )
        return 1.f;
    return 0.f;
}
