// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Math�matiques Appliqu�es SA (MASA)
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
#include "Tools.h"
#include "statusicons.h"
#include "protocol/Simulation.h"
#include "urban/TerrainObject_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Team_ABC& owner, const MsgsSimToClient::MsgUrbanKnowledgeCreation& message, Controller& controller,
                                const tools::Resolver_ABC< urban::TerrainObject_ABC >& terrainObjectResolver )
    : EntityImplementation< UrbanKnowledge_ABC >( controller, message.oid(), "" )
    , owner_                ( owner )
    , terrainObjectResolver_( terrainObjectResolver )
    , pRealUrban_           ( 0 )
{
    RegisterSelf( *this );
    pRealUrban_ = terrainObjectResolver_.Find( message.real_urban() );
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
    if( message.has_real_urban() )
        pRealUrban_ = terrainObjectResolver_.Find( message.real_urban() );
    if( message.has_relevance() )
        nRelevance_ = message.relevance();
    if( message.has_perceived() )
        bIsPerceived_ = message.perceived();
    if( message.has_identification_level() )
        nCurrrentPerceptionLevel_ = (E_PerceptionResult)( 3 - message.identification_level() );//@TODO MGD Fix enum
    if( message.has_progress() )
        nProgress_ = message.progress();
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
                .Display( tools::translate( "Urban", "Relevance:" ), nRelevance_ )
                .Display( tools::translate( "Urban", "Level:" ), nCurrrentPerceptionLevel_ )
                .Display( tools::translate( "Urban", "Progress:" ), nProgress_ ) ;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DisplayInList
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
void UrbanKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    if( pRealUrban_ )
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
    displayer.Display( tools::translate( "Urban", "Relevance:" ), nRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetName
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
QString UrbanKnowledge::GetName() const
{
    return pRealUrban_ ? QString( pRealUrban_->GetName().c_str() ) : tools::translate( "Urban", "Unknown urban object" );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetTypeName
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
QString UrbanKnowledge::GetTypeName() const
{
    return typeName_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetEntity
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
const Entity_ABC* UrbanKnowledge::GetEntity() const
{
    return 0;//@TODO MGD pRealUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetRecognizedEntity
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
const Entity_ABC* UrbanKnowledge::GetRecognizedEntity() const
{
    return 0;//pRealUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetOwner
// Created: MGD 2009-12-10
// -----------------------------------------------------------------------------
const Team_ABC& UrbanKnowledge::GetOwner() const
{
    return owner_;
}
