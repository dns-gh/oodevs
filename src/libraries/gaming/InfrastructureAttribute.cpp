// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "InfrastructureAttribute.h"
#include "Tools.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( kernel::Controller& controller, const gui::TerrainObjectProxy& object, const tools::StringResolver< InfrastructureType >& resolver, PropertiesDictionary& dictionary )
    : controller_     ( controller )
    , resolver_( resolver )
    , object_( object )
    , enabled_( true )
    , threshold_( 30 )
    , type_( 0 )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute destructor
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
InfrastructureAttribute::~InfrastructureAttribute()
{
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::CreateDictionary
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::CreateDictionary( kernel::PropertiesDictionary& dico ) const
{
    dico.Register( *this, tools::translate( "Block", "Infrastructure/Enable" ), enabled_ );
    dico.Register( *this, tools::translate( "Block", "Infrastructure/Threshold" ), threshold_ );
    dico.Register( *this, tools::translate( "Block", "Infrastructure/Role" ), role_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::DoUpdate
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::DoUpdate( const sword::UrbanUpdate& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::DoUpdate
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::DoUpdate( const sword::UrbanCreation& message )
{
    UpdateData( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::UpdateData
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
template< typename T >
void InfrastructureAttribute::UpdateData( const T& message )
{
    if( message.has_infrastructures() && message.infrastructures().has_infrastructure() )
    {
        enabled_ = message.infrastructures().infrastructure().active();
        threshold_ = message.infrastructures().infrastructure().threshold();
        role_ = message.infrastructures().infrastructure().type();
        type_ = resolver_.Find( role_ );
        controller_.Update( *static_cast< Infrastructure_ABC* >( this ) );
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Draw
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Draw( const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    if( type_ )
        tools.DrawApp6Symbol( type_->GetSymbol(), object_.Barycenter() , 0.1, 0.1 );
    if( !enabled_ )
        tools.DrawSvg( "infra.svg", object_.Barycenter(), 0.1 );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::IsEnable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::IsEnabled() const
{
    return enabled_;
}
