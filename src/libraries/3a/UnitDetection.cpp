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
// Name: UnitDetection::HasValue
// Created: SBO 2010-07-28
// -----------------------------------------------------------------------------
bool UnitDetection::HasValue( const sword::SimToClient& wrapper ) const
{
    if( wrapper.message().has_unit_detection() )
    {
        const sword::UnitDetection& message = wrapper.message().unit_detection();
        return detectedUnitId_ == static_cast< unsigned long >( message.detected_unit().id() );
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UnitDetection::Extract
// Created: SBO 2010-06-01
// -----------------------------------------------------------------------------
float UnitDetection::Extract( const sword::SimToClient& wrapper ) const
{
    const sword::UnitDetection& message = wrapper.message().unit_detection();
    if( detectedUnitId_ == static_cast< unsigned long >( message.detected_unit().id() ) && ( visibilityMask_ & ( 1 << message.current_visibility() ) ) != 0 )
        return 1.f;
    return 0.f;
}
