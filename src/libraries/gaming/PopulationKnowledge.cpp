// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "PopulationKnowledge.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const KnowledgeGroup_ABC& group,
                                          Controller& controller,
                                          const CoordinateConverter_ABC& converter,
                                          const tools::Resolver_ABC< Population_ABC >& resolver,
                                          const sword::CrowdKnowledgeCreation& message )
    : EntityImplementation< PopulationKnowledge_ABC >( controller, message.knowledge().id(), "", []( const PopulationKnowledge_ABC& ){ return false; } )
    , group_     ( group )
    , controller_( controller )
    , converter_ ( converter )
    , popu_      ( resolver.Get( message.crowd().id() ) )
    , domination_( 0 )
    , pTeam_     ( 0 )
{
    //$$ NLD - 2010-11-03 - Ce bloc sucks
    const Hierarchies* hierarchies = popu_.Retrieve< TacticalHierarchies >();
    if( ! hierarchies )
        hierarchies = popu_.Retrieve< CommunicationHierarchies >();
    const Entity_ABC& tmp = hierarchies ? hierarchies->GetTop() : popu_;
    pTeam_ = dynamic_cast< const kernel::Team_ABC* >( &tmp );

    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::~PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    tools::Resolver< PopulationFlowKnowledge >::DeleteAll();
    tools::Resolver< PopulationConcentrationKnowledge >::DeleteAll();
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
QString PopulationKnowledge::GetName() const
{
    return popu_.GetName();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdKnowledgeUpdate& message )
{
    if( message.has_domination() || message.has_critical_intelligence() )
    {
        if( message.has_domination() )
            domination_ = message.domination();
        if( message.has_critical_intelligence() )
            criticalIntelligence_ = message.critical_intelligence();
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdConcentrationKnowledgeCreation& message )
{
    if( ! tools::Resolver< PopulationConcentrationKnowledge >::Find( message.knowledge().id() ) )
    {
        PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( controller_, converter_, popu_, message );
        tools::Resolver< PopulationConcentrationKnowledge >::Register( message.knowledge().id(), *pKnowledge );
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate& message )
{
    tools::Resolver< PopulationConcentrationKnowledge >::Get( message.knowledge().id() )
        .DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdConcentrationKnowledgeDestruction& message )
{
    delete tools::Resolver< PopulationConcentrationKnowledge >::Find( message.knowledge().id() );
    tools::Resolver< PopulationConcentrationKnowledge >::Remove( message.knowledge().id() );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdFlowKnowledgeCreation& message )
{
    if( ! tools::Resolver< PopulationFlowKnowledge >::Find( message.knowledge().id() ) )
    {
        PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( controller_, converter_, popu_, message );
        tools::Resolver< PopulationFlowKnowledge >::Register( message.knowledge().id(), *pKnowledge );
        Touch();
    };
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdFlowKnowledgeUpdate& message )
{
    tools::Resolver< PopulationFlowKnowledge >::Get( message.knowledge().id() )
        .DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const sword::CrowdFlowKnowledgeDestruction& message )
{
    delete tools::Resolver< PopulationFlowKnowledge >::Find( message.knowledge().id() );
    tools::Resolver< PopulationFlowKnowledge >::Remove( message.knowledge().id() );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Crowd", "Known crowds" ) ).Start( popu_ ).Add( " - " ).Add( id_ ).End();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::Display( Displayer_ABC& displayer ) const
{
   displayer.Group( tools::translate( "Crowd", "Details" ) )
                .Display( tools::translate( "Crowd", "Identifier:" ), id_ )
                .Display( tools::translate( "Crowd", "Associated crowd:" ), popu_ )
                .Display( tools::translate( "Crowd", "Side:" ), popu_.Get< TacticalHierarchies >().GetTop() )
                .Display( tools::translate( "Crowd", "Critical intelligence:" ), criticalIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Crowd", "Side:" ), popu_.Get< TacticalHierarchies >().GetTop() )
             .Display( tools::translate( "Crowd", "Domination:" ), domination_ )
             .Display( tools::translate( "Crowd", "Critical intelligence:" ), criticalIntelligence_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetEntity
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const kernel::Population_ABC* PopulationKnowledge::GetEntity() const
{
    return &popu_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long PopulationKnowledge::GetEntityId() const
{
    return popu_.GetId();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetTeam
// Created: NLD 2010-11-03
// -----------------------------------------------------------------------------
const Team_ABC* PopulationKnowledge::GetTeam() const
{
    return pTeam_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetOwner
// Created: AGE 2006-10-16
// -----------------------------------------------------------------------------
const kernel::KnowledgeGroup_ABC& PopulationKnowledge::GetOwner() const
{
    return group_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Draw
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void PopulationKnowledge::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    const kernel::Positions& positions = Get< kernel::Positions >();
    if( viewport.IsVisible( positions.GetBoundingBox() ) )
    {
        {
            tools::Iterator< const PopulationFlowKnowledge& > it = tools::Resolver< PopulationFlowKnowledge >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
        {
            tools::Iterator< const PopulationConcentrationKnowledge& > it = tools::Resolver< PopulationConcentrationKnowledge >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
    }
}

void PopulationKnowledge::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}
