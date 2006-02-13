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
    , pType_        ( App::GetApp().GetAgentManager().FindTypePopulation( asnMsg.type_population ) )
    , pTeam_        ( & App::GetApp().GetModel().GetTeam( asnMsg.oid_camp ) )
{
    assert( pType_ );
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
Population::Population()
    : nPopulationID_( nMaxId_++ )
    , pType_        ( 0 )
    , pTeam_        ( 0 )
    , strName_      ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Population constructor
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
Population::Population( MT_Vector2D point, E_PopulationAttitude attitude, int persons, Team& team , std::string name, TypePopulation* type )
    : nPopulationID_( nMaxId_++ )
    , pType_        ( type )
    , pTeam_        ( &team )
    , strName_      ( name )
{
    CreatePopulationConcentration( point, attitude, persons );
}

// -----------------------------------------------------------------------------
// Name: Population destructor
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
Population::~Population()
{
	for( IT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		delete itCon->second;
	concentrationMap_.clear();

	for( IT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		delete itFlow->second;
	flowMap_.clear();
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulationFlow( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	flowMap_[ asnMsg.oid_flux ]->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulationConcentration( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	concentrationMap_[ asnMsg.oid_concentration ]->Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: Population::CreatePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::CreatePopulationFlow( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
	PopulationFlow* pFlow = new PopulationFlow( asnMsg, *this );
	flowMap_[ asnMsg.oid_flux ] = pFlow;
}

// -----------------------------------------------------------------------------
// Name: Population::CreatePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::CreatePopulationConcentration( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
	PopulationConcentration* pCon = new PopulationConcentration( asnMsg, *this );
	concentrationMap_[ asnMsg.oid_concentration ] = pCon ;
}

// -----------------------------------------------------------------------------
// Name: Population::CreatePopulationConcentration
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void Population::CreatePopulationConcentration( MT_Vector2D point, E_PopulationAttitude attitude, int persons )
{
    PopulationConcentration* pCon = new PopulationConcentration( point, attitude, persons, *this );
    concentrationMap_[ nMaxId_  ] = pCon ;
}

// -----------------------------------------------------------------------------
// Name: Population::DeletePopulationFlow			
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DeletePopulationFlow( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    IT_FlowMap it = flowMap_.find( asnMsg.oid_flux );
    assert( it != flowMap_.end() );
    App::GetApp().NotifyPopulationFlowDeleted( *it->second );
    flowMap_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Population::DeletePopulationConcentration	
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::DeletePopulationConcentration( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    IT_ConcentrationMap it = concentrationMap_.find( asnMsg.oid_concentration );
    assert( it != concentrationMap_.end() );
    App::GetApp().NotifyPopulationConcentrationDeleted( *it->second );
    concentrationMap_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: Population::UpdatePopulation				
// Created: HME 2005-09-29
// -----------------------------------------------------------------------------
void Population::UpdatePopulation( const ASN1T_MsgPopulationUpdate& /*asnMsg*/ )
{
} 


// -----------------------------------------------------------------------------
// Name: Population::MT_Vector2D& GetPos
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
const MT_Vector2D& Population::GetPos() const
{
	if ( concentrationMap_.size() == 0 && flowMap_.size() == 0 )
		assert( false );
	if( flowMap_.size() != 0 )
        return flowMap_.begin()->second->GetHeadPosition();
	else
        return concentrationMap_.begin()->second->GetPos();
		
}

// -----------------------------------------------------------------------------
// Name: Population::GetLivingHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint Population::GetLivingHumans() const
{
	uint sum = 0;
	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		sum += itCon->second->GetLivingHumans();
	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		sum += itFlow->second->GetLivingHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: Population::GetDeadHumans
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
uint Population::GetDeadHumans() const
{
	uint sum = 0;
	for( CIT_ConcentrationMap itCon = concentrationMap_.begin(); itCon != concentrationMap_.end(); ++itCon )
		sum += itCon->second->GetDeadHumans();
	for( CIT_FlowMap itFlow = flowMap_.begin(); itFlow != flowMap_.end(); ++itFlow )
		sum += itFlow->second->GetDeadHumans();
	return sum;
}

// -----------------------------------------------------------------------------
// Name: Population::ReadODB
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void Population::ReadODB( InputArchive& archive )
{
    archive.Section( "Population" );

    std::string strCategory;
    
    archive.ReadAttribute( "type", strCategory );
    pType_ = App::GetApp().GetAgentManager().FindTypePopulation( strCategory );
    assert( pType_ );

    archive.ReadAttribute( "id", nPopulationID_ );
    if( nPopulationID_ > nMaxId_ )
        nMaxId_ = nPopulationID_;

    if( ! archive.ReadField( "Nom", strName_, InputArchive::eNothing ) )
        strName_ = strCategory;

    std::string strPos;
    MT_Vector2D vPos;
    archive.ReadField( "Position", strPos );
    App::GetApp().GetWorld().MosToSimMgrsCoord( strPos, vPos );

    std::string strAtt;
    E_PopulationAttitude eAttitude;
    archive.ReadField( "Attitude", strAtt );
    eAttitude = ENT_Tr::ConvertToPopulationAttitude( strAtt );

    std::string strTeam;
    archive.ReadField( "Camp", strTeam );
    pTeam_ = App::GetApp().GetModel().FindTeam( strTeam ); // $$$$ AGE 2006-02-13: 

    uint nPersons;
    archive.ReadField( "NombreHumains", nPersons );

    CreatePopulationConcentration( vPos, eAttitude , nPersons );
    
    archive.EndSection();
}

// -----------------------------------------------------------------------------
// Name: Population::WriteODB
// Created: HME 2005-10-18
// -----------------------------------------------------------------------------
void Population::WriteODB( MT_XXmlOutputArchive& archive )
{
    archive.Section( "Population" );

    archive.WriteAttribute( "type", pType_->GetName() );
    archive.WriteAttribute( "id", nPopulationID_ );
    archive.WriteField    ( "Nom", strName_ );
    archive.WriteField    ( "Camp", pTeam_->GetName() );

    PopulationConcentration* pCon = concentrationMap_.begin()->second;
    std::string strPos;
    App::GetApp().GetWorld().SimToMosMgrsCoord( pCon->GetPos() , strPos );
    archive.WriteField( "Position"     , strPos );
    archive.WriteField( "NombreHumains", pCon->GetLivingHumans() );
    archive.WriteField( "Attitude"     , ENT_Tr::ConvertFromPopulationAttitude( pCon->GetAttitude() ) );
    
    archive.EndSection();
}

