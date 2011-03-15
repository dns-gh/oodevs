// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentAffinities.h"
#include "clients_kernel/Controllers.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( kernel::Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities constructor
// Created: LGY 2011-01-14
// -----------------------------------------------------------------------------
AgentAffinities::AgentAffinities( xml::xistream& xis, kernel::Controllers& controllers )
    : controllers_( controllers )
{
    xis >> xml::optional
            >> xml::start( "adhesions" )
                >> xml::list( "adhesion", *this, &AgentAffinities::ReadAffinity )
        >> xml::end;
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::ReadAffinity
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinities::ReadAffinity( xml::xistream& xis )
{
    affinities_[ xis.attribute< unsigned long >( "party" ) ] = xis.attribute< float >( "value" );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities destructor
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
AgentAffinities::~AgentAffinities()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::SerializeAttributes
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinities::SerializeAttributes( xml::xostream& xos ) const
{
    if( !affinities_.empty() )
    {
        xos << xml::start( "adhesions" );
        for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        {
            xos << xml::start( "adhesion" )
                    << xml::attribute( "party", it->first )
                    << xml::attribute( "value", it->second )
                << xml::end;
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Clear
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinities::Clear()
{
    affinities_.clear();
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::HasAffinities
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
bool AgentAffinities::HasAffinities() const
{
    return !affinities_.empty();
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Add
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinities::Add( unsigned long team, float value )
{
    affinities_[ team ] = value;
}

// -----------------------------------------------------------------------------
// Name: AgentAffinities::Fill
// Created: LGY 2011-03-14
// -----------------------------------------------------------------------------
void AgentAffinities::Fill( std::map< unsigned long, float >& content )
{
    for( CIT_Affinities it = affinities_.begin(); it != affinities_.end(); ++it )
        content[ it->first ] = it->second;
}
