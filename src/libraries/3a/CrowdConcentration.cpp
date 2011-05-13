// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "CrowdConcentration.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: CrowdConcentration constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdConcentration::CrowdConcentration()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdConcentration destructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdConcentration::~CrowdConcentration()
{
    // NOTHING
}

namespace
{
    const char* populationStates[ 4 ] =
    {
        "healthy",
        "wounded",
        "dead",
        "contaminated",
    };
    bool ReadMask( xml::xistream& xis, unsigned int index )
    {
        if( !xis.has_attribute( "states" ) )
            return true;
        FilterHelper< std::string > equipments( xis, "states" );
        bool result = false;
        if( equipments.IsAllowed( populationStates[index] ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: CrowdConcentration constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdConcentration::CrowdConcentration( xml::xistream& xis )
    : healthy_     ( ReadMask ( xis, 0 ) )
    , wounded_     ( ReadMask ( xis, 1 ) )
    , dead_        ( ReadMask ( xis, 2 ) )
    , contaminated_( ReadMask ( xis, 3 ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdConcentration::Extract
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
int CrowdConcentration::Extract( const sword::SimToClient& wrapper )
{
    int result = 0;
    const CrowdConcentrationUpdate& update = wrapper.message().crowd_concentration_update();

    if( healthy_.isAsked_ && update.has_healthy() )
        healthy_.quantity_ = update.healthy();
    if( wounded_.isAsked_ && update.has_wounded() )
        wounded_.quantity_ = update.wounded();
    if( dead_.isAsked_ && update.has_dead() )
        dead_.quantity_ = update.dead();
    if( contaminated_.isAsked_ && update.has_contaminated() )
        contaminated_.quantity_ = update.contaminated();

    result += healthy_.quantity_;
    result += wounded_.quantity_;
    result += dead_.quantity_;
    result += contaminated_.quantity_;

    return result;
}
