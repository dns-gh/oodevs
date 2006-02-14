// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "Population.h"

#include "App.h"
#include "AgentManager.h"
#include "Team.h"
#include "TypePopulation.h"
#include "World.h"

#include "PopulationFlow.h"
#include "PopulationConcentration.h"
#include "Model.h"

MIL_AgentID Population::nMaxId_ = 200;

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::Population( const ASN1T_MsgPopulationCreation& asnMsg )
    : nPopulationID_( asnMsg.oid_population )
    , strName_      ( asnMsg.nom )
    , pType_        ( 0 ) // App::GetApp().GetAgentManager().FindTypePopulation( asnMsg.type_population ) )
    , pTeam_        ( & App::GetApp().GetModel().GetTeam( asnMsg.oid_camp ) )
{
//    assert( pType_ );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
    Resolver< PopulationConcentration >::DeleteAll();
    Resolver< PopulationFlow >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulationFlow( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	Resolver< PopulationFlow >::Get( asnMsg.oid_flux ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulationConcentration( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	Resolver< PopulationConcentration >::Get( asnMsg.oid_concentration ).Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::CreatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::CreatePopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    Resolver< PopulationFlow >::Register( asnMsg.oid_flux, *new PopulationFlow( asnMsg, *this ) );
}

// -----------------------------------------------------------------------------
// Name: Population::CreatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::CreatePopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    Resolver< PopulationConcentration >::Register( asnMsg.oid_concentration, *new PopulationConcentration( asnMsg, *this ) );
}

// -----------------------------------------------------------------------------
// Name: Population::DeletePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DeletePopulationFlow( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    delete Resolver< PopulationFlow >::Find( asnMsg.oid_flux );
    Resolver< PopulationFlow >::Remove( asnMsg.oid_flux );
}

// -----------------------------------------------------------------------------
// Name: Population::DeletePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DeletePopulationConcentration( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    delete Resolver< PopulationConcentration >::Find( asnMsg.oid_concentration );
    Resolver< PopulationConcentration >::Remove( asnMsg.oid_concentration );
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulation				
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulation( const ASN1T_MsgPopulationUpdate& /*asnMsg*/ )
{
    // NOTHING
} 


// -----------------------------------------------------------------------------
// Name: Population::MT_Vector2D& GetPos
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D Population::GetPos() const
{
//	if ( concentrationMap_.size() == 0 && flowMap_.size() == 0 )
//		assert( false );
//	if( flowMap_.size() != 0 )
//        return flowMap_.begin()->second->GetHeadPosition();
//	else
//        return concentrationMap_.begin()->second->GetPos();
    return MT_Vector2D();
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint Population::GetLivingHumans() const
{
	uint sum = 0;
//	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
//		sum += itCon->second->GetLivingHumans();
//	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
//		sum += itFlow->second->GetLivingHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint Population::GetDeadHumans() const
{
	uint sum = 0;
//	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
//		sum += itCon->second->GetDeadHumans();
//	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
//		sum += itFlow->second->GetDeadHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: Population::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long Population::GetId() const
{
    return nPopulationID_;
}

// -----------------------------------------------------------------------------
// Name: Population::GetConcentration
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationConcentration& Population::GetConcentration( uint nID ) const
{
    return Resolver< PopulationConcentration >::Get( nID );
}

// -----------------------------------------------------------------------------
// Name: Population::GetFlow
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
const PopulationFlow& Population::GetFlow( uint nID ) const
{
    return Resolver< PopulationFlow >::Get( nID );
}

