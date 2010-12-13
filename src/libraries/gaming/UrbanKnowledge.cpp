// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "UrbanKnowledge.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "Tools.h"
#include "statusicons.h"
#include "protocol/Simulation.h"
#include <urban/TerrainObject_ABC.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Team_ABC& owner, const MsgsSimToClient::MsgUrbanKnowledgeCreation& message, Controller& controller,
                                const tools::Resolver< gui::TerrainObjectProxy >& terrainObjectResolver )
    : EntityImplementation< UrbanKnowledge_ABC >( controller, message.knowledge().id(), "" )
    , owner_                ( owner )
    , terrainObjectResolver_( terrainObjectResolver )
    , pRealUrban_           ( 0 )
{
    RegisterSelf( *this );
    pRealUrban_ = terrainObjectResolver_.Find( message.urban_block().id() );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge destructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledge::~UrbanKnowledge()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DoUpdate
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledge::DoUpdate( const MsgsSimToClient::MsgUrbanKnowledgeUpdate& message )
{
    if( message.urban_block().id() )
        pRealUrban_ = terrainObjectResolver_.Find( message.urban_block().id() );
    if( message.has_perceived() )
        bIsPerceived_ = message.perceived();
    if( message.has_progress() )
        rProgress_ = message.progress();
    if( message.has_maxprogress() )
        rMaxProgress_ = message.maxprogress();
    Touch();
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Display
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Urban", "Details" ) )
                .Display( tools::translate( "Urban", "Identifier:" ), id_ )
                .Display( tools::translate( "Urban", "Associated block:" ), pRealUrban_ ? pRealUrban_->GetObject() : static_cast< urban::TerrainObject_ABC* >( 0 ) )
                .Display( tools::translate( "Urban", "Perceived:" ), bIsPerceived_ )
                .Display( tools::translate( "Urban", "Progress:" ), rProgress_ )
                .Display( tools::translate( "Urban", "Maximum Progress:" ), rMaxProgress_ ) ;

}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DisplayInList
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealUrban_ && pRealUrban_->GetName().length() )
        displayer.Display( tools::translate( "Urban", "Known blocks" ), pRealUrban_->GetObject() );
    else
        displayer.Display( tools::translate( "Urban", "Known blocks" ), id_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DisplayInSummary
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledge::DisplayInSummary( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Urban", "Progress:" ), rProgress_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetName
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
QString UrbanKnowledge::GetName() const
{
    if( pRealUrban_ )
    {
        std::string name = pRealUrban_->GetName();
        if ( name.empty() )
            return QString( tools::translate( "Urban", "Urban block[%1]" ).arg( pRealUrban_->GetId() ) );
        return QString( pRealUrban_->GetName().ascii() );
    }
    return tools::translate( "Urban", "Unknown urban object" );

}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetEntity
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
const Entity_ABC* UrbanKnowledge::GetEntity() const
{
    return pRealUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetTeam
// Created: NLD 2009-11-03
// -----------------------------------------------------------------------------
const Team_ABC* UrbanKnowledge::GetTeam() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetOwner
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
const Team_ABC& UrbanKnowledge::GetOwner() const
{
    return owner_;
}
