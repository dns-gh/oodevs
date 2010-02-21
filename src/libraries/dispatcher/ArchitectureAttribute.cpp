// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ArchitectureAttribute.h"
#include "protocol/Simulation.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute constructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::ArchitectureAttribute( const Model& model, const MsgsSimToClient::MsgUrbanAttributes& message )
: UrbanObjectAttribute_ABC( model, message )
, height_ ( 0. )
, floorNumber_     ( 0 )
, basementLevelNumber_ ( 0 )
, roofShape_ ( "" )
, material_ ( "" )
, innerCluttering_ ( 0. )
, facadeOpacity_ ( 0. )
{
    Update( message );
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute destructor
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
ArchitectureAttribute::~ArchitectureAttribute()
{

}  

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Update
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Update( const MsgsSimToClient::MsgUrbanAttributes& message )
{
    if( message.has_architecture() )
    {
        if ( message.architecture().has_height() )
            height_ = message.architecture().height();
        if ( message.architecture().has_floor_number() )
            floorNumber_ = message.architecture().floor_number();
        if ( message.architecture().has_basement_level_number() )
            basementLevelNumber_ = message.architecture().basement_level_number();  
        if ( message.architecture().has_roof_shape() )
            roofShape_ = message.architecture().roof_shape(); 
        if ( message.architecture().has_material() )
            material_ = message.architecture().material(); 
        if ( message.architecture().has_inner_cluttering() )
            innerCluttering_ = message.architecture().inner_cluttering(); 
        if ( message.architecture().has_facade_opacity() )
            facadeOpacity_ = message.architecture().facade_opacity(); 
    }
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::Send
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::Send( MsgsSimToClient::MsgUrbanAttributes& message ) const
{
    message.mutable_architecture()->set_height( height_ );
    message.mutable_architecture()->set_floor_number( floorNumber_ );
    message.mutable_architecture()->set_basement_level_number( basementLevelNumber_ );
    message.mutable_architecture()->set_roof_shape( roofShape_.c_str() );
    message.mutable_architecture()->set_material( material_.c_str() );
    message.mutable_architecture()->set_inner_cluttering( innerCluttering_ );
    message.mutable_architecture()->set_facade_opacity( facadeOpacity_ );
}

// -----------------------------------------------------------------------------
// Name: ArchitectureAttribute::AsnDelete
// Created: SLG 2009-12-04
// -----------------------------------------------------------------------------
void ArchitectureAttribute::AsnDelete( MsgsSimToClient::MsgUrbanAttributes& /*message*/ ) const
{
    //    delete asnMsg.u.mine_jam;
}
