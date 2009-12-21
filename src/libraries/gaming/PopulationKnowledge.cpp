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
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Viewport_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const KnowledgeGroup_ABC& group, Controller& controller, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< Population_ABC >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message )
    : EntityImplementation< PopulationKnowledge_ABC >( controller, message.oid_connaissance, "" )
    , group_     ( group )
    , controller_( controller )
    , converter_ ( converter )
    , popu_      ( resolver.Get( message.oid_population_reelle ) )
    , domination_( 0 )
{
    RegisterSelf( *this );
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
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    if( message.m.etat_dominationPresent )
    {
        domination_ = message.etat_domination;
        Touch();
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    if( ! tools::Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration ) )
    {
        PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( controller_, converter_, popu_, message );
        tools::Resolver< PopulationConcentrationKnowledge >::Register( message.oid_connaissance_concentration, *pKnowledge );
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    tools::Resolver< PopulationConcentrationKnowledge >::Get( message.oid_connaissance_concentration )
        .DoUpdate( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    delete tools::Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration );
    tools::Resolver< PopulationConcentrationKnowledge >::Remove( message.oid_connaissance_concentration );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeCreation& message )
{
    if( ! tools::Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux ) )
    {
        PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( controller_, converter_, popu_, message );
        tools::Resolver< PopulationFlowKnowledge >::Register( message.oid_connaissance_flux, *pKnowledge );
        Touch();
    };
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeUpdate& message )
{
    tools::Resolver< PopulationFlowKnowledge >::Get( message.oid_connaissance_flux )
        .DoUpdate( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeDestruction& message )
{
    delete tools::Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux );
    tools::Resolver< PopulationFlowKnowledge >::Remove( message.oid_connaissance_flux );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Item( tools::translate( "Population", "Known populations" ) ).Start( popu_ ).Add( " - " ).Add( id_ ).End();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::Display( Displayer_ABC& displayer ) const
{
   displayer.Group( tools::translate( "Population", "Details" ) )
                .Display( tools::translate( "Population", "Identifier:" ), id_ )
                .Display( tools::translate( "Population", "Associated population:" ), popu_ )
                .Display( tools::translate( "Population", "Side:" ), popu_.Get< TacticalHierarchies >().GetTop() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInSummary
// Created: SBO 2007-05-14
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Population", "Side:" ), popu_.Get< TacticalHierarchies >().GetTop() );
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
// Name: PopulationKnowledge::GetRecognizedEntity
// Created: SBO 2006-12-08
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* PopulationKnowledge::GetRecognizedEntity() const
{
    return &popu_;
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
void PopulationKnowledge::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const GlTools_ABC& tools ) const
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

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Accept
// Created: MGD 2009-12-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
