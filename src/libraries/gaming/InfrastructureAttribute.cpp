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
#include "clients_kernel/Controllers.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( Controllers& controllers, gui::TerrainObjectProxy& object, const tools::StringResolver< InfrastructureType >& resolver, PropertiesDictionary& dictionary )
    : controllers_( controllers )
    , resolver_   ( resolver )
    , object_     ( object )
    , position_   ( object.Get< kernel::UrbanPositions_ABC >().Barycenter() )
    , enabled_    ( true )
    , threshold_  ( 30 )
    , type_       ( 0 )
{
    CreateDictionary( dictionary );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute destructor
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
InfrastructureAttribute::~InfrastructureAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::CreateDictionary
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::CreateDictionary( PropertiesDictionary& dico ) const
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
        threshold_ = static_cast< unsigned int >( message.infrastructures().infrastructure().threshold() * 100 + 0.5 );
        role_ = message.infrastructures().infrastructure().type();
        type_ = resolver_.Find( role_ );
        controllers_.controller_.Update( object_ );
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Draw
// Created: SLG 2011-01-14
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Draw( const Viewport_ABC& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsHotpointVisible() )
    {
        if( type_ )
            tools.DrawApp6Symbol( type_->GetSymbol(), position_, 0.1f, 0.1f );
        if( !enabled_ )
            tools.DrawSvg( "infra.svg", position_, 0.1f );
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::IsEnable
// Created: SLG 2011-01-18
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::GetThreshold
// Created: CMA 2011-09-08
// -----------------------------------------------------------------------------
unsigned int InfrastructureAttribute::GetThreshold() const
{
    return threshold_;
}
