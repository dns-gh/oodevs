// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "SoilAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SoilAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
SoilAttribute::SoilAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message )
: UrbanObjectAttribute_ABC( model, message )
, occupation_       ( 0. )
, trafficability_   ( 0. )
, isMultiple_       ( false )
, compoundClearing_ ("")
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
SoilAttribute::~SoilAttribute()
{
    //NOTHING
}  

// -----------------------------------------------------------------------------
// Name: SoilAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& message )
{
    if( message.has_soil() )
    {
        if ( message.soil().has_occupation() )
            occupation_ = message.soil().occupation();
        if ( message.soil().has_trafficability() )
            trafficability_ = message.soil().trafficability();
        if ( message.soil().has_multiple() )
            isMultiple_ = message.soil().multiple();
        if ( message.soil().has_compound_clearing() )
            compoundClearing_ = message.soil().compound_clearing();  
    }
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
 
    message.mutable_soil()->set_occupation( occupation_ );
    message.mutable_soil()->set_trafficability( trafficability_ );
    message.mutable_soil()->set_multiple( isMultiple_ );
    message.mutable_soil()->set_compound_clearing( compoundClearing_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: SoilAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void SoilAttribute::AsnDelete( MsgsSimToClient::MsgUrbanAttributes& /*message*/ ) const
{
    //NOTHING
}
