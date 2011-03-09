// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InitialStateCrew.h"
#include <xeumeuleu/xml.hpp>

// =============================================================================
// Enum conversion
// =============================================================================

namespace
{
    static InitialStateCrew::E_CrewRanks CrewRanksStringToEnum( const std::string& value )
    {
        if( value == "Officier" )
            return InitialStateCrew::eOfficer;
        else if( value == "SousOfficier" )
            return InitialStateCrew::eWarrant;
        else if( value == "Crew" )
            return InitialStateCrew::ePrivate;
        assert( false );
        return InitialStateCrew::eOfficer;
    }
    static const std::string CrewRanksEnumToString( const InitialStateCrew::E_CrewRanks& value )
    {
        if( value == InitialStateCrew::eOfficer )
            return "Officier";
        else if( value == InitialStateCrew::eWarrant )
            return "SousOfficier";
        else if( value == InitialStateCrew::ePrivate )
            return "Crew";
        assert( false );
        return "";
    }

    static InitialStateCrew::E_CrewStates CrewStatesStringToEnum( const std::string& value )
    {
        if( value == "healthy" )
            return InitialStateCrew::eHealthy;
        else if( value == "ue" )
            return InitialStateCrew::eWounded_ue;
        else if( value == "u1" )
            return InitialStateCrew::eWounded_u1;
        else if( value == "u2" )
            return InitialStateCrew::eWounded_u2;
        else if( value == "u3" )
            return InitialStateCrew::eWounded_u3;
        else if( value == "dead" )
            return InitialStateCrew::eDead;
        assert( false );
        return InitialStateCrew::eHealthy;
    }
    static const std::string CrewStatesEnumToString( const InitialStateCrew::E_CrewStates& value )
    {
        if( value == InitialStateCrew::eHealthy )
            return "healthy";
        else if( value == InitialStateCrew::eWounded_ue )
            return "ue";
        else if( value == InitialStateCrew::eWounded_u1 )
            return "u1";
        else if( value == InitialStateCrew::eWounded_u2 )
            return "u2";
        else if( value == InitialStateCrew::eWounded_u3 )
            return "u3";
        else if( value == InitialStateCrew::eDead )
            return "dead";
        assert( false );
        return "";
    }
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::InitialStateCrew
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateCrew::InitialStateCrew( E_CrewRanks rank, E_CrewStates state, bool psyop, bool contaminated, unsigned int number )
    : rank_        ( rank )
    , state_       ( state )
    , psyop_       ( psyop )
    , contaminated_( contaminated )
    , number_      ( number )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::InitialStateCrew
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateCrew::InitialStateCrew( xml::xistream& xis )
{
    std::string rank;
    std::string state;
    xis >> xml::attribute( "rank", rank )
        >> xml::attribute( "state", state )
        >> xml::attribute( "psyop", psyop_ )
        >> xml::attribute( "contaminated", contaminated_ )
        >> xml::attribute( "number", number_ );
    rank_ = CrewRanksStringToEnum( rank );
    state_ = CrewStatesStringToEnum( state );
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::~InitialStateCrew
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateCrew::~InitialStateCrew()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::Serialize
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
void InitialStateCrew::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "human" )
        << xml::attribute( "rank", CrewRanksEnumToString( rank_ ) )
        << xml::attribute( "state", CrewStatesEnumToString( state_ ) )
        << xml::attribute( "psyop", psyop_ )
        << xml::attribute( "contaminated", contaminated_ )
        << xml::attribute( "number", number_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::operator==
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateCrew::operator==( const InitialStateCrew& object ) const
{
    return rank_ == object.rank_ && state_ == object.state_ && psyop_ == object.psyop_ && contaminated_ == object.contaminated_ && number_ == object.number_;
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::operator!=
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateCrew::operator!=( const InitialStateCrew& object ) const
{
    return !( *this == object );
}
