//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: PopulationKnowledge.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledge.h"

#include "KnowledgeGroup.h"
#include "Population.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "Controller.h"
#include "Displayer_ABC.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( Controller& controller, const CoordinateConverter& converter, const Resolver_ABC< Population >& resolver, const ASN1T_MsgPopulationKnowledgeCreation& message )
    : controller_( controller )
    , converter_ ( converter )
    , nID_       ( message.oid_connaissance )
    , popu_      ( resolver.Get( message.oid_population_reelle ) )
{
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
void PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& )
{
    // message.etat_domination // $$$$ AGE 2006-03-13: 
    // NOTHING
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
   displayer.Group( "D�tails" )
                .Display( "Id:", nID_ )
                .Display( "Population associ�e:", popu_ )
                .Display( "Camp:", popu_.GetTeam() );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::IsInTeam
// Created: AGE 2006-03-13
// -----------------------------------------------------------------------------
bool PopulationKnowledge::IsInTeam( const Team& team ) const
{
    return & popu_.GetTeam() == &team;
}
