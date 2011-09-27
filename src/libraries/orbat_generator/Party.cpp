// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************
#include "orbat_generator_pch.h"
#include "Party.h"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <xeumeuleu/xml.hpp>

using namespace orbat_generator;

namespace
{
    const std::string PartySideToString( const PartySide& side )
    {
        if( side == Friend )
            return "friend";
        if( side == Enemy )
            return "enemy";
        if( side == Neutral )
            return "neutral";
        throw std::runtime_error( "PartySide error" );
    }
}

// -----------------------------------------------------------------------------
// Name: Party constructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Party::Party( const PartySide& side )
    : side_   ( PartySideToString( side ) )
    , current_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party destructor
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
Party::~Party()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party::InsertIntoOrbat
// Created: RCD 2011-03-02
// -----------------------------------------------------------------------------
void Party::InsertIntoOrbat( xml::xostream& orbat, const IdNameGenerator& idNameGen )
{
    unsigned int id = idNameGen.ComputePartyId();
    std::string name = idNameGen.ComputePartyName();
    orbat << xml::start( "party" )
            << xml::attribute< int >( "id", id )
            << xml::attribute< std::string >( "name", name )
            << xml::attribute< std::string >( "type", side_ )
            << xml::start( "objects" )
            << xml::end
            << xml::start( "tactical" );
    for ( unsigned int it = 0; it < formations_.size(); ++it )
        formations_[ it ]->InsertIntoOrbat( orbat, idNameGen );
    orbat   << xml::end
            << xml::start( "communication" );
    unsigned int knowledgeId = idNameGen.ComputeKnowledgeId() - 1;
	for( unsigned int it = 0; it < formations_.size(); ++it )
	{
        orbat     << xml::start( "knowledge-group" )
                    << xml::attribute< unsigned int >( "id", 1000 * ( knowledgeId - it ) )
                    << xml::attribute< std::string >( "name", "Groupe de connaissance" )
                    << xml::attribute< std::string >( "type", "Standard" )
                  << xml::end;
	}
    orbat   << xml::end
            << xml::start( "populations" )
            << xml::end
            << xml::start( "inhabitants" )
            << xml::end
            << xml::start( "logistics" )
            << xml::end
          << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Party::GenerateDiplomacy
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
void Party::GenerateDiplomacy( xml::xostream& orbat, const unsigned int id, std::vector< boost::shared_ptr< Party > > parties )
{
    orbat << xml::start( "party" )
            << xml::attribute( "id", id + 1 );
    for( unsigned int it = 0; it < parties.size(); ++it )
    {
        if( it != id )
            parties[ it ]->GenerateDiplomacy( orbat, it, side_ );
    }
    orbat << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Party::GenerateDiplomacy
// Created: RCD 2011-03-07
// -----------------------------------------------------------------------------
void Party::GenerateDiplomacy( xml::xostream& orbat, const unsigned int id, const std::string& side )
{
    orbat << xml::start( "relationship" );
    if( side_ == "neutral" || side == "neutral" )
        orbat << xml::attribute( "diplomacy", "neutral" );
    else if( side_ == side )
        orbat << xml::attribute( "diplomacy", "friend" );
    else
        orbat << xml::attribute( "diplomacy", "enemy" );
    orbat << xml::attribute( "party", id+1 )
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Party::AddFormation
// Created: RCD 2011-03-04
// -----------------------------------------------------------------------------
void Party::AddFormation( boost::shared_ptr< Formation > formation )
{
    formations_.push_back( formation );
}

// -----------------------------------------------------------------------------
// Name: Party::AddAutomate
// Created: RCD 2011-03-04
// -----------------------------------------------------------------------------
void Party::AddAutomate( boost::shared_ptr< Automate > automate )
{
    formations_[ current_ ]->AddAutomate( automate );
    current_ = ++current_ % formations_.size();
}
