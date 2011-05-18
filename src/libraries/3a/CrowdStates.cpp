// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "CrowdStates.h"
#include "MT_Tools\MT_Logger.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: CrowdStates constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdStates::CrowdStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdStates destructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdStates::~CrowdStates()
{
    // NOTHING
}

namespace
{
    bool ReadMask( xml::xistream& xis, const std::string& state )
    {
        if( !xis.has_attribute( "states" ) )
            return true;
        FilterHelper< std::string > states( xis, "states" );
        bool result = false;
        if( states.IsAllowed( state ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: CrowdStates constructor
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
CrowdStates::CrowdStates( xml::xistream& xis )
    : healthy_     ( ReadMask ( xis, "healthy" ) )
    , wounded_     ( ReadMask ( xis, "wounded" ) )
    , dead_        ( ReadMask ( xis, "dead" ) )
    , contaminated_( ReadMask ( xis, "contaminated" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdStates::Extract
// Created: FPO 2011-05-13
// -----------------------------------------------------------------------------
int CrowdStates::Extract( const sword::SimToClient& wrapper )
{
    int result = 0;
    const CrowdUpdate& update = wrapper.message().crowd_update();

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
