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
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Team_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const KnowledgeGroup_ABC& group, Controller& controller, const CoordinateConverter_ABC& converter, const Resolver_ABC< Population_ABC >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message )
    : group_     ( group )
    , controller_( controller )
    , converter_ ( converter )
    , nID_       ( message.oid_connaissance )
    , popu_      ( resolver.Get( message.oid_population_reelle ) )
    , domination_( 0 )
{
    RegisterSelf( *this );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::~PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    Resolver< PopulationFlowKnowledge >::DeleteAll();
    Resolver< PopulationConcentrationKnowledge >::DeleteAll();
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long PopulationKnowledge::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetName
// Created: AGE 2006-03-15
// -----------------------------------------------------------------------------
std::string PopulationKnowledge::GetName() const
{
    return popu_.GetName();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    if( message.m.etat_dominationPresent )
        domination_ = message.etat_domination;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    if( ! Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration ) )
    {
        PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( controller_, converter_, popu_, message );
        Resolver< PopulationConcentrationKnowledge >::Register( message.oid_connaissance_concentration, *pKnowledge );
    };
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    Resolver< PopulationConcentrationKnowledge >::Get( message.oid_connaissance_concentration )
        .Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    delete Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration );
    Resolver< PopulationConcentrationKnowledge >::Remove( message.oid_connaissance_concentration );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    if( ! Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux ) )
    {
        PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( controller_, converter_, popu_, message );
        Resolver< PopulationFlowKnowledge >::Register( message.oid_connaissance_flux, *pKnowledge );
    };
    controller_.Update( *this );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    Resolver< PopulationFlowKnowledge >::Get( message.oid_connaissance_flux )
        .Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    delete Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux );
    Resolver< PopulationFlowKnowledge >::Remove( message.oid_connaissance_flux );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Item( "Populations connues" ).Start( popu_ ).Add( " - " ).Add( nID_ ).End();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationKnowledge::Display( Displayer_ABC& displayer ) const
{
   displayer.Group( "Détails" )
                .Display( "Id:", nID_ )
                .Display( "Population associée:", popu_ )
                .Display( "Camp:", popu_.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool PopulationKnowledge::IsInTeam( const Team_ABC& team ) const
{
     return group_.IsInTeam( team );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::KnowledgeIsInTeam
// Created: AGE 2006-05-17
// -----------------------------------------------------------------------------
bool PopulationKnowledge::KnowledgeIsInTeam( const Team_ABC& team ) const
{
    return popu_.GetTeam() == team;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetKnowledgeGroup
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const KnowledgeGroup_ABC& PopulationKnowledge::GetKnowledgeGroup() const
{
    return group_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetRealPopulation
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const Population_ABC& PopulationKnowledge::GetRealPopulation() const
{
    return popu_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetKnowledgeTeam
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const Team_ABC* PopulationKnowledge::GetKnowledgeTeam() const
{
    return & popu_.GetTeam();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Draw
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void PopulationKnowledge::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    // $$$$ AGE 2006-05-18: 
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Select
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Select( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02: 
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::ContextMenu
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::ContextMenu( ActionController& controller, const QPoint& where ) const
{
    // $$$$ SBO 2006-08-02: 
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Activate
// Created: SBO 2006-08-02
// -----------------------------------------------------------------------------
void PopulationKnowledge::Activate( ActionController& controller ) const
{
    // $$$$ SBO 2006-08-02: 
}
