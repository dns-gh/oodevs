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
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const KnowledgeGroup_ABC& group, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Population_ABC >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message )
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
    Resolver< PopulationFlowKnowledge >::DeleteAll();
    Resolver< PopulationConcentrationKnowledge >::DeleteAll();
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
// Name: PopulationKnowledge::GetTypeName
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
QString PopulationKnowledge::GetTypeName() const
{
    return typeName_;
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
    if( ! Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration ) )
    {
        PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( controller_, converter_, popu_, message );
        Resolver< PopulationConcentrationKnowledge >::Register( message.oid_connaissance_concentration, *pKnowledge );
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    Resolver< PopulationConcentrationKnowledge >::Get( message.oid_connaissance_concentration )
        .DoUpdate( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    delete Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration );
    Resolver< PopulationConcentrationKnowledge >::Remove( message.oid_connaissance_concentration );
    Touch();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    if( ! Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux ) )
    {
        PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( controller_, converter_, popu_, message );
        Resolver< PopulationFlowKnowledge >::Register( message.oid_connaissance_flux, *pKnowledge );
        Touch();
    };
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    Resolver< PopulationFlowKnowledge >::Get( message.oid_connaissance_flux )
        .DoUpdate( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DoUpdate
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    delete Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux );
    Resolver< PopulationFlowKnowledge >::Remove( message.oid_connaissance_flux );
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
//    if( viewport.IsVisible( boundingBox_ ) ) // $$$$ SBO 2007-02-27: 
    {
        {
            Iterator< const PopulationFlowKnowledge& > it = Resolver< PopulationFlowKnowledge >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
        {
            Iterator< const PopulationConcentrationKnowledge& > it = Resolver< PopulationConcentrationKnowledge >::CreateIterator();
            while( it.HasMoreElements() )
                it.NextElement().Draw( where, viewport, tools );
        }
    }
}
