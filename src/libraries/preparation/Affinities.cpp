// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Affinities.h"
#include "Model.h"
#include "TeamsModel.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
Affinities::Affinities( Model& model )
    : model_( model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities constructor
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
Affinities::Affinities( xml::xistream& xis, Model& model )
    : model_( model )
{
    xis >> xml::optional
            >> xml::start( "adhesions" )
                >> xml::list( "adhesion", *this, &Affinities::ReadAffinity )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Affinities::~AgentAffinities
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
Affinities::~Affinities()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Affinities::ReadAffinity
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::ReadAffinity( xml::xistream& xis )
{
    affinities_[ xis.attribute< unsigned long >( "party" ) ] = xis.attribute< float >( "value" );
}

// -----------------------------------------------------------------------------
// Name: Affinities::Add
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::Add( unsigned long team, float affinity )
{
    affinities_[ team ] = affinity;
}

// -----------------------------------------------------------------------------
// Name: Affinities::Fill
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::Fill( std::map< unsigned long, float >& content )
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        content[ it->first ] = it->second;
}

// -----------------------------------------------------------------------------
// Name: Affinities::HasAffinities
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
bool Affinities::HasAffinities() const
{
    return !affinities_.empty();
}

// -----------------------------------------------------------------------------
// Name: Affinities::SerializeAttributes
// Created: LGY 2011-03-17
// -----------------------------------------------------------------------------
void Affinities::SerializeAttributes( xml::xostream& xos ) const
{
    if( !affinities_.empty() )
    {
        xos << xml::start( "adhesions" );
        for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        {
            if( model_.GetTeamResolver().Find( it->first ) )
            {
                xos << xml::start( "adhesion" )
                    << xml::attribute( "party", it->first )
                    << xml::attribute( "value", it->second )
                    << xml::end;
            }
        }
        xos << xml::end;
    }
}