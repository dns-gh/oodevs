// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Order_ABC.h"
#include "MissionParameter_ABC.h"
#include "protocol/protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Order_ABC constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::Order_ABC( Model_ABC& model, unsigned int missionID, const Common::MsgMissionParameters& parameters )
    : model_( model )
    , missionID_ ( missionID )
    , parameters_()
{
    InitializeParameters( parameters );
}

// -----------------------------------------------------------------------------
// Name: Order_ABC destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::~Order_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::InitializeParameters
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::InitializeParameters( const Common::MsgMissionParameters& parameters )
{
    for( int i = 0; i < parameters.elem_size(); ++i )
        parameters_.push_back( MissionParameter_ABC::Create( parameters.elem( i ) ) );
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::Send( Common::MsgMissionParameters& asn ) const
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        if( *it )
            (*it)->Send( *asn.add_elem() );
        else
            asn.add_elem()->set_null_value( true );
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::Delete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::Delete( Common::MsgMissionParameters& asn ) const
{
    asn.Clear();
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::GetId
// Created: SBO 2009-10-05
// -----------------------------------------------------------------------------
unsigned int Order_ABC::GetId() const
{
    return missionID_;
}
