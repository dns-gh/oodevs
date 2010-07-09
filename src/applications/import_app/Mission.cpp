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
// Name: Mission::Set
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Mission::Set( const std::string& missionId, const std::vector< std::vector< Position > >& tacticals )
{
    id_ = missionId;
    tacticals_ = tacticals;
}

// -----------------------------------------------------------------------------
// Name: Mission::Write
// Created: LDC 2010-07-09
// -----------------------------------------------------------------------------
void Mission::Write( xml::xostream& xos, Mapping& mapping )
{
    xos << xml::attribute( "id", mapping[ id_ ] )
        << xml::attribute( "name", "" );
    int i = 0;
    for( std::vector< std::vector< Position > >::const_iterator it = tacticals_.begin(); it != tacticals_.end(); ++it, ++i )
    {
        xos << xml::start( "parameter" )
                << xml::attribute( "name", mapping.GetMissionParameterName( i ) )
                << xml::attribute( "type", mapping.GetMissionParameterType( i ) )
                << xml::start( "location" )
                << xml::attribute( "type", mapping.GetMissionParameterLocationType( i ) );
        for( std::vector< Position >::const_iterator itPos = it->begin(); itPos != it->end(); ++itPos )
        {
            xos << xml::start( "point" );
            itPos->WriteAttribute( "coordinates", xos );
            xos << xml::end();
        }
        xos     << xml::end()
            << xml::end();
    }
}