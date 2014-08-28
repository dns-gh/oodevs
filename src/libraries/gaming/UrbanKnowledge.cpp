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
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Team_ABC& owner,
                                const sword::UrbanKnowledgeCreation& message,
                                Controller& controller,
                                const tools::Resolver< kernel::UrbanObject_ABC >& terrainObjectResolver )
    : EntityImplementation< UrbanKnowledge_ABC >( controller, message.knowledge().id(), "", []( const kernel::Entity_ABC& ){ return false; } )
    , terrainObjectResolver_( terrainObjectResolver )
    , owner_                ( owner )
    , pRealUrban_           ( terrainObjectResolver_.Find( message.object().id() ) )
    , entityId_             ( 0 )
{
    if( pRealUrban_ )
        entityId_ = pRealUrban_->GetId();
    AddExtension( *this );
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
void UrbanKnowledge::DoUpdate( const sword::UrbanKnowledgeUpdate& message )
{
    if( message.object().id() )
    {
        pRealUrban_ = terrainObjectResolver_.Find( message.object().id() );
        if( pRealUrban_ )
            entityId_ = pRealUrban_->GetId();
    }
    if( message.has_perceived() )
        bIsPerceived_ = message.perceived();
    if( message.has_progress() )
        rProgress_ = message.progress();
    if( message.has_max_progress() )
        rMaxProgress_ = message.max_progress();
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
                .Display( tools::translate( "Urban", "Associated block:" ), pRealUrban_ )
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
        displayer.Display( tools::translate( "Urban", "Known blocks" ), pRealUrban_ );
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
        return pRealUrban_->GetName();
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
// Name: UrbanKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long UrbanKnowledge::GetEntityId() const
{
    return entityId_;
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
