// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "esri_plugin_pch.h"
#include "StatusListener.h"
#include "Database_ABC.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"

using namespace plugins;
using namespace plugins::esri;

// -----------------------------------------------------------------------------
// Name: StatusListener constructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::StatusListener( Database_ABC& database, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_      ( publisher )
//    , propertiesTable_( database.OpenTable( "SimulationProperties" ) )
    , database_       ( database )
    , paused_         ( false )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: StatusListener destructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::~StatusListener()
{
    // NOTHING
}

namespace
{
    template< typename Type >
    Type GetField( const Row_ABC& row, const std::string& name )
    {
        return boost::get< Type >( row.GetField( name ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StatusListener::Listen
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::Listen()
{
    std::auto_ptr< Table_ABC > properties( database_.OpenTable( "SimulationProperties" ) );
    Row_ABC* result = properties->Find( "Property='Status'" );
    if( result )
        ChangeStatus( GetField< std::string >( *result, "PropertyValue" ) );
}

// -----------------------------------------------------------------------------
// Name: StatusListener::ChangeStatus
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
void StatusListener::ChangeStatus( const std::string& status )
{
    if( paused_ && status == "resumed" )
    {
        simulation::ControlResume asn;
        asn.Send( publisher_ );
        paused_ = false;
    }
    else if( !paused_ && status == "paused" )
    {
        simulation::ControlPause asn;
        asn.Send( publisher_ );
        paused_ = true;
    }
}
