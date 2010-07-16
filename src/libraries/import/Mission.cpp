// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "Mission.h"
#include "Mapping.h"

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::Mission()
    : mapping_( 0 ) // $$$$ MCO : hmm !
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission constructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::Mission( const std::string& id, const std::vector< std::vector< Position > >& tacticals, Mapping& mapping )
    : id_       ( id )
    , tacticals_( tacticals )
    , mapping_  ( &mapping )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission destructor
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
Mission::~Mission()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: operator<<
// Created: MCO 2010-07-13
// -----------------------------------------------------------------------------
xml::xostream& operator<<( xml::xostream& xos, const Mission& mission )
{
    xos << xml::attribute( "id", (*mission.mapping_)[ mission.id_ ] )
        << xml::attribute( "name", "" );
    int i = 0;
    for( std::vector< std::vector< Position > >::const_iterator it = mission.tacticals_.begin(); it != mission.tacticals_.end(); ++it, ++i )
    {
        xos << xml::start( "parameter" )
                << xml::attribute( "name", mission.mapping_->GetMissionParameterName( i ) )
                << xml::attribute( "type", mission.mapping_->GetMissionParameterType( i ) )
                << xml::start( "location" )
                    << xml::attribute( "type", mission.mapping_->GetMissionParameterLocationType( i ) );
        for( std::vector< Position >::const_iterator itPos = it->begin(); itPos != it->end(); ++itPos )
            xos << xml::start( "point" )
                    << xml::attribute( "coordinates", *itPos )
                << xml::end;
        xos     << xml::end
            << xml::end;
    }
    return xos;
}
