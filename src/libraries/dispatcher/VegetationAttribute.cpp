// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "VegetationAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: VegetationAttribute constructor
// Created: SLG 2009-12-04 
// -----------------------------------------------------------------------------
VegetationAttribute::VegetationAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message  )
    : UrbanObjectAttribute_ABC( model, message )
    , type_     ( "" )
    , height_   ( 0 )
    , density_  ( 0. )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
VegetationAttribute::~VegetationAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& message  )
{
    if( message.has_vegetation() )
    {
        type_       = message.vegetation().type();
        height_     = message.vegetation().height();
        density_    = message.vegetation().density();        
    }
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message  ) const
{
    message.mutable_vegetation()->set_type(  type_.c_str() );
    message.mutable_vegetation()->set_height( height_  );
    message.mutable_vegetation()->set_density( density_ );
}

// -----------------------------------------------------------------------------
// Name: VegetationAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void VegetationAttribute::AsnDelete( MsgsSimToClient::MsgUrbanAttributes& /*message*/ ) const
{
}
