// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "StatusListener.h"
#include "Connector.h"
#include "Table_ABC.h"
#include "Row_ABC.h"
#include "dispatcher/SimulationPublisher_ABC.h"
#include "dispatcher/Network_Def.h"

using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: StatusListener constructor
// Created: JCR 2007-06-13
// -----------------------------------------------------------------------------
StatusListener::StatusListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher )
    : publisher_      ( publisher )
    , propertiesTable_( connector.GetTable( "SimulationProperties" ) )
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
    Row_ABC* result = propertiesTable_.Find( "Property='Status'" );
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
        dispatcher::AsnMsgClientToSimControlResume asn;
        asn.Send( publisher_ );
        paused_ = false;
    }
    else if( !paused_ && status == "paused" )
    {
        dispatcher::AsnMsgClientToSimControlPause asn;
        asn.Send( publisher_ );
        paused_ = true;
    }
}
