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
#include "ENT/ENT_Tr.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )

namespace
{
    // $$$$ ABR 2011-07-20: Temporaire en attente de l'histoire 660
    void LoadInjury( const std::string& value, E_HumanState& state, E_InjuriesSeriousness& injurySeriousness )
    {
        injurySeriousness = eInjuriesSeriousness_U1;
        state = eHumanState_Injured;

        if( value == "healthy" )
            state = eHumanState_Healthy;
        else if( value == "ue" )
            injurySeriousness = eInjuriesSeriousness_UE;
        else if( value == "u1" )
            injurySeriousness = eInjuriesSeriousness_U1;
        else if( value == "u2" )
            injurySeriousness = eInjuriesSeriousness_U2;
        else if( value == "u3" )
            injurySeriousness = eInjuriesSeriousness_U3;
        else if( value == "dead" )
            state = eHumanState_Dead;
    }
    std::string SaveInjury( E_HumanState state, E_InjuriesSeriousness injurySeriousness )
    {
        if( state == eHumanState_Healthy )
            return "healthy";
        else if( state == eHumanState_Injured )
        {
            if( injurySeriousness == eInjuriesSeriousness_UE )
                return "ue";
            else if( injurySeriousness == eInjuriesSeriousness_U1 )
                return "u1";
            else if( injurySeriousness == eInjuriesSeriousness_U2 )
                return "u2";
            else if( injurySeriousness == eInjuriesSeriousness_U3 )
                return "u3";
        }
        else if( state == eHumanState_Dead )
            return "dead";
        assert( false );
        return "healhty";
    }
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::InitialStateCrew
// Created: ABR 2011-03-02
// -----------------------------------------------------------------------------
InitialStateCrew::InitialStateCrew( E_HumanRank rank, E_HumanState state, E_InjuriesSeriousness injurySeriousness, bool psyop, bool contaminated, unsigned int number )
    : rank_        ( rank )
    , state_       ( state )
    , currentSeriousness_( injurySeriousness )
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
    std::string injuries;
    xis >> xml::attribute( "rank", rank )
        >> xml::attribute( "state", state )
        >> xml::optional >> xml::attribute( "injuries", injuries )
        >> xml::attribute( "psyop", psyop_ )
        >> xml::attribute( "contaminated", contaminated_ )
        >> xml::attribute( "number", number_ );
    rank_ = ENT_Tr::ConvertToHumanRank( rank );
    //rank_ = tools::HumanRankFromString( rank );

    LoadInjury( state, state_, currentSeriousness_ ); // $$$$ ABR 2011-07-20: Temporaire en attente de l'histoire 660
    //state_ = tools::HumanStateFromString( state );
    LoadInjuries( injuries );// $$$$ ABR 2011-07-20: En avance sur l'histoire 660
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
        << xml::attribute( "rank", ENT_Tr::ConvertFromHumanRank( rank_, ENT_Tr_ABC::eToSim ) )
        //<< xml::attribute( "rank", tools::ToString( rank_ ) )
        << xml::attribute( "state", SaveInjury( state_, currentSeriousness_ ) );
        //<< xml::attribute( "state", tools::ToString( state_ ) );

    if( !injuries_.empty() )
        xos << xml::attribute( "injuries", SaveInjuries() );// $$$$ ABR 2011-07-20: En avance sur l'histoire 660

    xos << xml::attribute( "psyop", psyop_ )
        << xml::attribute( "contaminated", contaminated_ )
        << xml::attribute( "number", number_ )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::LoadInjuries
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
void InitialStateCrew::LoadInjuries( const std::string& injuries )
{
    std::vector< std::string > injuriesList;
    boost::split( injuriesList, injuries, boost::algorithm::is_any_of( ";" ) );
//     for( std::vector< std::string >::const_iterator it = injuriesList.begin(); it != injuriesList.end(); ++it )
//     {
//         unsigned int id = 0;
//         E_InjuriesSeriousness seriousness = eInjuriesSeriousness_U1;
//         //injuries_[ id ] = seriousness; // $$$$ ABR 2011-07-19: FIXME
//     }
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::SaveInjuries
// Created: ABR 2011-07-19
// -----------------------------------------------------------------------------
const std::string InitialStateCrew::SaveInjuries() const
{
    std::string result;
//     for( CIT_Injuries it = injuries_.begin(); it != injuries_.end(); ++it )
//     {
//         unsigned int id = it->first;
//         E_InjuriesSeriousness seriousness = it->second;
//         //result << id << seriousness; // $$$$ ABR 2011-07-19: FIXME
//     }
    return result;
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::operator==
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateCrew::operator==( const InitialStateCrew& object ) const
{
    return rank_ == object.rank_ &&
           state_ == object.state_ &&
           currentSeriousness_ == object.currentSeriousness_ && // $$$$ ABR 2011-07-20: Temporaire en attente de l'histoire 660
           psyop_ == object.psyop_ &&
           contaminated_ == object.contaminated_ &&
           number_ == object.number_;
}

// -----------------------------------------------------------------------------
// Name: InitialStateCrew::operator!=
// Created: ABR 2011-03-03
// -----------------------------------------------------------------------------
bool InitialStateCrew::operator!=( const InitialStateCrew& object ) const
{
    return !( *this == object );
}
